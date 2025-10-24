package completions

import (
	"context"
	"net/http"
	"strings"
	"time"

	"code-completion/pkg/codebase_context"
	"code-completion/pkg/config"
	"code-completion/pkg/metrics"
	"code-completion/pkg/model"
)

// 补全处理器
type CompletionHandler struct {
	contextClient *codebase_context.ContextClient
	model         *model.OpenAIModel // 模型
}

type CompletionContext struct {
	Ctx  context.Context
	Perf *CompletionPerformance
}

// prompt处理结果
type PromptResult struct {
	Prompt       string `json:"prompt"`
	PromptTokens int    `json:"prompt_tokens"`
	NewPrefix    string `json:"new_prefix"`
	NewSuffix    string `json:"new_suffix"`
}

func NewCompletionContext(ctx context.Context, perf *CompletionPerformance) *CompletionContext {
	return &CompletionContext{
		Ctx:  ctx,
		Perf: perf,
	}
}

// NewCompletionHandler 创建新的补全处理器
func NewCompletionHandler() *CompletionHandler {
	m := model.GlobalModelManager.GetModel()
	return &CompletionHandler{
		contextClient: codebase_context.NewContextClient(),
		model:         m,
	}
}

// 处理补全请求
func (h *CompletionHandler) HandleCompletion(c *CompletionContext, req *CompletionRequest, headers http.Header) *CompletionResponse {
	// startTime := time.Now()
	c.Perf.StartTime = time.Now()

	// 0. 补全拒绝规则链处理
	rejectResult := DefaultCompletionRejectRuleChain.Handle(req)
	if rejectResult != nil {
		totalDuration := float64(time.Since(c.Perf.ReceiveTime).Milliseconds())
		metrics.RecordCompletionTotalDuration(req.Model, metrics.StatusReject, totalDuration)
		metrics.IncrementCompletionRequests(req.Model, metrics.StatusReject)

		return RejectRequest(req)
	}

	// 1. 解析请求参数
	prefix, suffix, linePrefix, lineSuffix, codeContext := h.parsePrompt(req)

	// 2. 获取上下文信息
	if codeContext == "" {
		codeContext = h.getContext(c.Ctx, req, prefix, suffix, headers)
	}
	// 3. 补全前置处理 （拼接prompt策略，单行/多行补全策略）
	promptResult := h.preparePrompt(h.model, prefix, suffix, codeContext)

	// 4. 判断是否为单行补全
	isSingleCompletion := h.judgeSingleCompletion(linePrefix, lineSuffix, req.LanguageID)
	if isSingleCompletion {
		if len(req.Stop) > 0 {
			req.Stop = append(req.Stop, "\n")
		} else {
			req.Stop = []string{"\n"}
		}
	}

	// 5. 准备停用词
	stopWords := h.prepareStopWords(suffix, req)

	// 6. 交给模型处理
	completionData := map[string]interface{}{
		"prompt":            promptResult.Prompt,
		"prompt_tokens":     promptResult.PromptTokens,
		"stop":              stopWords,
		"temperature":       req.Temperature,
		"max_tokens":        h.model.Config.MaxOuputToken,
		"presence_penalty":  0.0,
		"frequency_penalty": 0.0,
		"top_p":             1,
		"stream":            false,
	}
	// completionDatas := model.CompletionRequest{}
	c.Perf.ModelStartTime = time.Now()
	result, completionStatus, reqStartTime, reqEndTime := h.model.Completions(c.Ctx, completionData, req.CompletionID)
	c.Perf.ModelEndTime = time.Now()

	// 记录模型推理耗时
	var modelDuration float64
	if reqStartTime != nil && reqEndTime != nil {
		modelDuration = float64(reqEndTime.Sub(*reqStartTime).Milliseconds())
	} else {
		modelDuration = float64(time.Since(c.Perf.ReceiveTime).Milliseconds())
	}

	if completionStatus != model.CompletionSuccess {
		// 记录模型错误指标
		status := metrics.Status(completionStatus)
		totalDuration := float64(time.Since(c.Perf.ReceiveTime).Milliseconds())
		metrics.RecordCompletionTotalDuration(req.Model, status, totalDuration)
		metrics.RecordCompletionModelDuration(req.Model, status, modelDuration)
		metrics.IncrementCompletionRequests(req.Model, status)
		metrics.RecordCompletionTokens(metrics.TokenTypeInput, req.Model, promptResult.PromptTokens)

		return ErrorResponse(req, completionStatus, c.Perf, promptResult.Prompt, []CompletionChoice{})
	}

	// 6. 补全后置处理
	var completionText string
	var modelChoices []CompletionChoice

	// 从模型结果中获取补全文本，这里需要根据实际的模型返回结构进行调整
	if result != nil {
		if choices, ok := result["choices"].([]interface{}); ok && len(choices) > 0 {
			// 获取第一个choice作为主要补全文本
			if choice, ok := choices[0].(map[string]interface{}); ok {
				if text, ok := choice["text"].(string); ok {
					completionText = text
				}
			}

			// 提取所有choices作为ModelChoices
			for _, choiceInterface := range choices {
				if choice, ok := choiceInterface.(map[string]interface{}); ok {
					if text, ok := choice["text"].(string); ok {
						modelChoices = append(modelChoices, CompletionChoice{Text: text})
					}
				}
			}
		}
	}

	// 创建补全后置处理器，处理补全结果
	if completionText != "" {
		postprocessorContext := &PostprocessorContext{
			Language:       req.LanguageID,
			CompletionCode: completionText,
			Prefix:         prefix,
			Suffix:         suffix,
		}

		chain := NewDefaultPostprocessorChain(req.CompletionID)
		chain.Process(postprocessorContext)

		// 去除补全内容后空格
		completionText = strings.TrimSpace(postprocessorContext.CompletionCode)
	}

	if completionText == "" {
		// 记录空结果指标
		totalDuration := float64(time.Since(c.Perf.ReceiveTime).Milliseconds())
		metrics.RecordCompletionTotalDuration(req.Model, metrics.StatusEmpty, totalDuration)
		metrics.RecordCompletionModelDuration(req.Model, metrics.StatusEmpty, modelDuration)
		metrics.IncrementCompletionRequests(req.Model, metrics.StatusEmpty)
		metrics.RecordCompletionTokens(metrics.TokenTypeInput, req.Model, promptResult.PromptTokens)

		return ErrorResponse(req, model.CompletionEmpty, c.Perf, promptResult.Prompt, modelChoices)
	}

	// 7. 构建响应
	response := SuccessResponse(req, completionText, startTime, promptResult.Prompt, promptResult.PromptTokens,
		reqStartTime, reqEndTime, 0, modelChoices)

	// 记录成功指标
	totalDuration := float64(time.Since(c.Perf.ReceiveTime).Milliseconds())
	metrics.RecordCompletionTotalDuration(req.Model, metrics.StatusSucess, totalDuration)
	metrics.RecordCompletionModelDuration(req.Model, metrics.StatusSucess, modelDuration)
	metrics.IncrementCompletionRequests(req.Model, metrics.StatusSucess)
	metrics.RecordCompletionTokens(metrics.TokenTypeInput, req.Model, promptResult.PromptTokens)
	metrics.RecordCompletionTokens(metrics.TokenTypeOutput, req.Model, len(completionText))

	return response
}

// parsePrompt 解析提示词
func (h *CompletionHandler) parsePrompt(req *CompletionRequest) (prefix, suffix, linePrefix, lineSuffix, codeContext string) {
	if req.PromptOptions != nil {
		prefix = req.PromptOptions.Prefix
		suffix = req.PromptOptions.Suffix
		linePrefix = req.PromptOptions.CursorLinePrefix
		lineSuffix = req.PromptOptions.CursorLineSuffix
		codeContext = req.PromptOptions.CodeContext
	} else {
		// 简单的提示词解析逻辑，参考Python代码
		prefix = req.Prompt
		// 可以根据FIM_INDICATOR分割prompt来获取prefix和suffix
	}

	// 如果linePrefix为空，从prefix中提取
	if linePrefix == "" && prefix != "" {
		lines := strings.Split(prefix, "\n")
		if len(lines) > 0 {
			linePrefix = lines[len(lines)-1]
		}
	}

	// 如果lineSuffix为空，从suffix中提取
	if lineSuffix == "" && suffix != "" {
		lines := strings.Split(suffix, "\n")
		if len(lines) > 0 {
			lineSuffix = lines[0]
			if len(lines) > 1 {
				lineSuffix += "\n"
			}
		}
	}

	return prefix, suffix, linePrefix, lineSuffix, codeContext
}

// getPromptTemplate 获取prompt模板
func (h *CompletionHandler) getPromptTemplate(prefix, suffix, codeContext string, modelConfig *config.ModelConfig) string {
	return modelConfig.FimPrefix + codeContext + "\n" + prefix + modelConfig.FimSuffix + suffix + modelConfig.FimMiddle
}

// 准备停用词
func (h *CompletionHandler) prepareStopWords(suffix string, req *CompletionRequest) []string {
	var stopWords []string

	// 添加请求中的停用词
	if len(req.Stop) > 0 {
		stopWords = append(stopWords, req.Stop...)
	}

	// 添加默认的FIM停用词
	stopWords = append(stopWords, "<｜end▁of▁sentence｜>")

	// 如果后缀为空，添加系统停用词
	if suffix == "" || strings.TrimSpace(suffix) == "" {
		stopWords = append(stopWords, "\n\n", "\n\n\n")
	}

	return stopWords
}

// 获取上下文信息
func (h *CompletionHandler) getContext(ctx context.Context, req *CompletionRequest, prefix, suffix string, headers http.Header) string {
	return h.contextClient.GetContext(
		ctx,
		req.ClientID,
		req.ProjectPath,
		req.FileProjectPath,
		prefix,
		suffix,
		req.ImportContent,
		headers,
	)
}
