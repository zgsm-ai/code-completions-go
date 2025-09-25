package completions

import (
	"context"
	"net/http"
	"strings"
	"time"

	"code-completions/pkg/codebase_context"
	"code-completions/pkg/metrics"
	"code-completions/pkg/model"

	"github.com/gin-gonic/gin"
	"go.uber.org/zap"
)

// CalculateHideScore 计算隐藏分数配置
type CalculateHideScore struct {
	IsWhitespaceAfterCursor bool   `json:"is_whitespace_after_cursor"`
	Prefix                  string `json:"prefix"`
	DocumentLength          int    `json:"document_length"`
	PromptEndPos            int    `json:"prompt_end_pos"`
	PreviousLabel           int    `json:"previous_label"`
	PreviousLabelTimestamp  int64  `json:"previous_label_timestamp"`
}

// CompletionHandler 补全处理器
type CompletionHandler struct {
	contextClient *codebase_context.ContextClient
	model         *model.OpenAIModel // 模型
}

// PromptResult prompt处理结果
type PromptResult struct {
	Prompt       string `json:"prompt"`
	PromptTokens int    `json:"prompt_tokens"`
	NewPrefix    string `json:"new_prefix"`
	NewSuffix    string `json:"new_suffix"`
}

// NewCompletionHandler 创建新的补全处理器
func NewCompletionHandler() *CompletionHandler {
	m := model.GlobalModelManager.GetModel()
	if m == nil {
		return nil
	}
	return &CompletionHandler{
		contextClient: codebase_context.NewContextClient(),
		model:         m,
	}
}

// HandleCompletion 处理补全请求
func (h *CompletionHandler) HandleCompletion(ctx context.Context, req *CompletionRequest, headers map[string]string) (*CompletionResponse, error) {
	requestTime := time.Now()
	startTime := time.Now()

	// 0. 补全拒绝规则链处理
	rejectResult := DefaultCompletionRejectRuleChain.Handle(req)
	if rejectResult != nil {
		// 记录拒绝指标
		totalDuration := float64(time.Since(startTime).Milliseconds())
		metrics.RecordCompletionTotalDuration(req.Model, metrics.StatusReject, totalDuration)
		metrics.IncrementCompletionRequests(req.Model, metrics.StatusReject)

		return ErrorResponse(req.CompletionID, req.Model, model.CompletionReject, requestTime, "", 0,
			nil, nil, 0, []CompletionChoice{}), nil
	}

	// 1. 解析请求参数
	prefix, suffix, cursorLinePrefix, cursorLineSuffix, codeContext := h.parsePrompt(req)

	// 2. 获取上下文信息
	if codeContext == "" {
		context, err := h.getContext(ctx, req, prefix, suffix, headers)
		if err != nil {
			return nil, err
		}
		codeContext = context
	}

	// 3. 补全前置处理 （拼接prompt策略，单行/多行补全策略）
	promptResult, err := h.preparePrompt(h.model, prefix, suffix, codeContext)
	if err != nil {
		return nil, err
	}

	// 4. 判断是否为单行补全
	isSingleCompletion := h.judgeSingleCompletion(cursorLinePrefix, cursorLineSuffix, req.LanguageID)
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
	result, completionStatus, reqStartTime, reqEndTime := h.model.Completions(ctx, completionData, "")

	// 记录模型推理耗时
	var modelDuration float64
	if reqStartTime != nil && reqEndTime != nil {
		modelDuration = float64(reqEndTime.Sub(*reqStartTime).Milliseconds())
	} else {
		modelDuration = float64(time.Since(startTime).Milliseconds())
	}

	if completionStatus != model.CompletionSucess {
		zap.L().Error("请求模型补全错误", zap.String("Status", string(completionStatus)))

		// 记录模型错误指标
		status := metrics.Status(completionStatus)
		totalDuration := float64(time.Since(startTime).Milliseconds())
		metrics.RecordCompletionTotalDuration(req.Model, status, totalDuration)
		metrics.RecordCompletionModelDuration(req.Model, status, modelDuration)
		metrics.IncrementCompletionRequests(req.Model, status)
		metrics.RecordCompletionTokens(metrics.TokenTypeInput, req.Model, promptResult.PromptTokens)

		return ErrorResponse(req.CompletionID, req.Model, completionStatus, requestTime, promptResult.Prompt, promptResult.PromptTokens,
			reqStartTime, reqEndTime, 0, []CompletionChoice{}), nil
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
		postprocessorContext := &CompletionPostprocessorContext{
			Language:       req.LanguageID,
			CompletionCode: completionText,
			Prefix:         prefix,
			Suffix:         suffix,
		}

		factory := &CompletionPostprocessorFactory{}
		postprocessorChain := factory.CreateDefault(req.CompletionID)
		postprocessorChain.Process(postprocessorContext)

		// 无实际补全内容，置为空
		if !isValidContent(postprocessorContext.CompletionCode) {
			postprocessorContext.CompletionCode = ""
		}

		// 去除补全内容后空格
		completionText = strings.TrimSpace(postprocessorContext.CompletionCode)
	}

	if completionText == "" {
		// 记录空结果指标
		totalDuration := float64(time.Since(startTime).Milliseconds())
		metrics.RecordCompletionTotalDuration(req.Model, metrics.StatusEmpty, totalDuration)
		metrics.RecordCompletionModelDuration(req.Model, metrics.StatusEmpty, modelDuration)
		metrics.IncrementCompletionRequests(req.Model, metrics.StatusEmpty)
		metrics.RecordCompletionTokens(metrics.TokenTypeInput, req.Model, promptResult.PromptTokens)

		return ErrorResponse(req.CompletionID, req.Model, model.CompletionProcessEmpty, requestTime, promptResult.Prompt, promptResult.PromptTokens,
			reqStartTime, reqEndTime, 0, modelChoices), nil
	}

	// 7. 构建响应
	response := SuccessResponse(req.CompletionID, req.Model, completionText, requestTime, promptResult.Prompt, promptResult.PromptTokens,
		reqStartTime, reqEndTime, 0, modelChoices)

	// 记录成功指标
	totalDuration := float64(time.Since(startTime).Milliseconds())
	metrics.RecordCompletionTotalDuration(req.Model, metrics.StatusSucess, totalDuration)
	metrics.RecordCompletionModelDuration(req.Model, metrics.StatusSucess, modelDuration)
	metrics.IncrementCompletionRequests(req.Model, metrics.StatusSucess)
	metrics.RecordCompletionTokens(metrics.TokenTypeInput, req.Model, promptResult.PromptTokens)
	metrics.RecordCompletionTokens(metrics.TokenTypeOutput, req.Model, len(completionText))

	return response, nil
}

// parsePrompt 解析提示词
func (h *CompletionHandler) parsePrompt(req *CompletionRequest) (prefix, suffix, cursorLinePrefix, cursorLineSuffix, codeContext string) {
	if req.PromptOptions != nil {
		prefix = req.PromptOptions.Prefix
		suffix = req.PromptOptions.Suffix
		cursorLinePrefix = req.PromptOptions.CursorLinePrefix
		cursorLineSuffix = req.PromptOptions.CursorLineSuffix
		codeContext = req.PromptOptions.CodeContext
	} else {
		// 简单的提示词解析逻辑，参考Python代码
		prefix = req.Prompt
		// 可以根据FIM_INDICATOR分割prompt来获取prefix和suffix
	}

	// 如果cursorLinePrefix为空，从prefix中提取
	if cursorLinePrefix == "" && prefix != "" {
		lines := strings.Split(prefix, "\n")
		if len(lines) > 0 {
			cursorLinePrefix = lines[len(lines)-1]
		}
	}

	// 如果cursorLineSuffix为空，从suffix中提取
	if cursorLineSuffix == "" && suffix != "" {
		lines := strings.Split(suffix, "\n")
		if len(lines) > 0 {
			cursorLineSuffix = lines[0]
			if len(lines) > 1 {
				cursorLineSuffix += "\n"
			}
		}
	}

	return prefix, suffix, cursorLinePrefix, cursorLineSuffix, codeContext
}

// getPromptTemplate 获取prompt模板
func (h *CompletionHandler) getPromptTemplate(prefix, suffix, codeContext string, modelConfig *model.ModelConfig) string {
	return modelConfig.FimPrefix + codeContext + "\n" + prefix + modelConfig.FimSuffix + suffix + modelConfig.FimMiddle
}

// prepareStopWords 准备停用词
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

// getContext 获取上下文信息
func (h *CompletionHandler) getContext(ctx context.Context, req *CompletionRequest, prefix, suffix string, headers map[string]string) (string, error) {
	// 使用上下文客户端获取上下文信息
	context, err := h.contextClient.GetContext(
		ctx,
		req.ClientID,
		req.ProjectPath,
		req.FileProjectPath,
		prefix,
		suffix,
		req.ImportContent,
		headers,
	)
	if err != nil {
		return "", err
	}
	return context, nil
}

// GinHandler Gin框架的处理器
func (h *CompletionHandler) GinHandler(c *gin.Context) {
	// 解析请求
	var req CompletionRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	// 获取请求头
	headers := make(map[string]string)
	for key, values := range c.Request.Header {
		if len(values) > 0 {
			headers[key] = values[0]
		}
	}

	// 处理补全请求
	response, err := h.HandleCompletion(c.Request.Context(), &req, headers)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// 返回响应
	c.JSON(http.StatusOK, response)
}
