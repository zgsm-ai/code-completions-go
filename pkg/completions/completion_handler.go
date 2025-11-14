package completions

import (
	"context"
	"fmt"
	"time"

	"code-completion/pkg/config"
	"code-completion/pkg/model"
)

// 补全处理器
type CompletionHandler struct {
	cfg *config.ModelConfig // 模型配置
	llm model.LLM           // 模型
}

type CompletionContext struct {
	Ctx  context.Context
	Perf *CompletionPerformance
}

func NewCompletionContext(ctx context.Context, perf *CompletionPerformance) *CompletionContext {
	return &CompletionContext{
		Ctx:  ctx,
		Perf: perf,
	}
}

// 创建新的补全处理器
func NewCompletionHandler(m model.LLM) *CompletionHandler {
	if m == nil {
		m = model.GetAutoModel()
	}
	return &CompletionHandler{
		llm: m,
		cfg: m.Config(),
	}
}

// 处理补全请求
func (h *CompletionHandler) CallLLM(c *CompletionContext, input *CompletionInput) *CompletionResponse {
	// 3. 补全模型相关的前置处理 （拼接prompt策略，单行/多行补全策略，裁剪过长上下文）
	h.truncatePrompt(h.cfg, &input.Processed)

	// 4. 准备停用词，根据是否单行补全调整停用词
	stopWords := h.prepareStopWords(input)

	// 5. 交给模型处理
	var para model.CompletionParameter
	para.Model = input.Model
	para.ClientID = input.ClientID
	para.CompletionID = input.CompletionID
	para.Prefix = input.Processed.Prefix
	para.Suffix = input.Processed.Suffix
	para.CodeContext = input.Processed.CodeContext
	para.Stop = stopWords
	para.MaxTokens = h.cfg.MaxOutputToken
	para.Temperature = float32(input.Temperature)

	modelStartTime := time.Now().Local()
	rsp, verbose, completionStatus, err := h.llm.Completions(c.Ctx, &para)
	modelEndTime := time.Now().Local()
	c.Perf.LLMDuration = modelEndTime.Sub(modelStartTime)

	if completionStatus != model.StatusSuccess {
		c.Perf.PromptTokens = h.getTokensCount(input.Processed.Prefix) + h.getTokensCount(input.Processed.CodeContext)
		c.Perf.TotalDuration = time.Since(c.Perf.ReceiveTime)
		return ErrorResponse(input, completionStatus, c.Perf, verbose, err)
	}

	// 6. 补全后置处理
	var completionText string
	if len(rsp.Choices) > 0 {
		completionText = rsp.Choices[0].Text
	}
	if completionText != "" && !h.cfg.DisablePrune {
		completionText = h.pruneCompletionCode(completionText, para.Prefix, para.Suffix, input.LanguageID)
	}
	c.Perf.PromptTokens = rsp.Usage.PromptTokens
	c.Perf.CompletionTokens = rsp.Usage.CompletionTokens
	c.Perf.TotalTokens = c.Perf.CompletionTokens + c.Perf.PromptTokens
	c.Perf.TotalDuration = time.Since(c.Perf.ReceiveTime)

	if completionText == "" {
		return ErrorResponse(input, model.StatusEmpty, c.Perf, verbose, fmt.Errorf("empty"))
	}

	// 7. 构建响应
	return SuccessResponse(input, completionText, c.Perf, verbose)
}

// 完整处理补全请求
func (h *CompletionHandler) HandleCompletion(c *CompletionContext, input *CompletionInput) *CompletionResponse {
	rsp := input.Preprocess(c)
	if rsp != nil {
		return rsp
	}
	return h.CallLLM(c, input)
}
