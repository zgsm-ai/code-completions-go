package completions

import (
	"context"
	"fmt"
	"net/http"
	"time"

	"code-completion/pkg/codebase_context"
	"code-completion/pkg/config"
	"code-completion/pkg/metrics"
	"code-completion/pkg/model"
)

// 补全处理器
type CompletionHandler struct {
	contextClient *codebase_context.ContextClient
	cfg           *config.ModelConfig
	llm           model.LLM // 模型
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
		contextClient: codebase_context.NewContextClient(),
		llm:           m,
		cfg:           m.Config(),
	}
}

// 处理补全请求
func (h *CompletionHandler) HandleCompletion(c *CompletionContext, req *CompletionRequest, headers http.Header) *CompletionResponse {
	c.Perf.StartTime = time.Now().Local()
	c.Perf.QueueDuration = time.Since(c.Perf.ReceiveTime)

	var para model.CompletionParameter
	// 0. 补全拒绝规则链处理
	err := NewFilterChain(&config.Config.CompletionsConfig).Handle(req)
	if err != nil {
		c.Perf.TotalDuration = time.Since(c.Perf.ReceiveTime)
		metrics.RecordCompletionDuration(h.cfg.ModelName, metrics.StatusReject,
			c.Perf.QueueDuration, c.Perf.ContextDuration,
			c.Perf.LLMDuration, c.Perf.TotalDuration)
		metrics.IncrementCompletionRequests(h.cfg.ModelName, metrics.StatusReject)
		return RejectRequest(req, c.Perf, err)
	}

	// 1. 解析请求参数
	ppt := h.parsePrompt(req)

	// 2. 获取上下文信息
	if ppt.CodeContext == "" {
		ppt.CodeContext = h.getContext(c.Ctx, req, ppt.Prefix, ppt.Suffix, headers)
		c.Perf.ContextDuration = time.Since(c.Perf.StartTime)
	}
	// 3. 补全前置处理 （拼接prompt策略，单行/多行补全策略）
	h.truncatePrompt(h.cfg, ppt)

	// 4. 准备停用词，根据是否单行补全调整停用词
	stopWords := h.prepareStopWords(ppt, req)

	// 5. 交给模型处理
	para.Model = req.Model
	para.ClientID = req.ClientID
	para.CompletionID = req.CompletionID
	para.Prefix = ppt.Prefix
	para.Suffix = ppt.Suffix
	para.CodeContext = ppt.CodeContext
	para.Stop = stopWords
	para.MaxTokens = h.cfg.MaxOuputToken
	para.Temperature = float32(req.Temperature)

	c.Perf.ModelStartTime = time.Now().Local()
	rsp, dbg, completionStatus, err := h.llm.Completions(c.Ctx, &para)
	c.Perf.ModelEndTime = time.Now().Local()
	c.Perf.LLMDuration = c.Perf.ModelEndTime.Sub(c.Perf.ModelStartTime)

	if completionStatus != model.CompletionSuccess {
		c.Perf.TotalDuration = time.Since(c.Perf.ReceiveTime)
		c.Perf.PromptTokens = h.getTokensCount(ppt.Prefix) + h.getTokensCount(ppt.CodeContext)
		status := metrics.Status(completionStatus)
		metrics.RecordCompletionDuration(h.cfg.ModelName, status,
			c.Perf.QueueDuration, c.Perf.ContextDuration,
			c.Perf.LLMDuration, c.Perf.TotalDuration)
		metrics.IncrementCompletionRequests(h.cfg.ModelName, status)
		metrics.RecordCompletionTokens(h.cfg.ModelName, metrics.TokenTypeInput, c.Perf.PromptTokens)
		metrics.RecordCompletionTokens(h.cfg.ModelName, metrics.TokenTypeOutput, 0)
		return ErrorResponse(req, completionStatus, c.Perf, dbg, err)
	}

	// 6. 补全后置处理
	var completionText string
	if len(rsp.Choices) > 0 {
		completionText = rsp.Choices[0].Text
	}
	if completionText != "" && !h.cfg.DisablePrune {
		completionText = h.pruneCompletionCode(completionText, ppt.Prefix, ppt.Suffix, req.LanguageID)
	}
	c.Perf.PromptTokens = rsp.Usage.PromptTokens
	c.Perf.CompletionTokens = rsp.Usage.CompletionTokens
	c.Perf.TotalTokens = c.Perf.CompletionTokens + c.Perf.PromptTokens
	c.Perf.TotalDuration = time.Since(c.Perf.ReceiveTime)

	if completionText == "" {
		metrics.RecordCompletionDuration(h.cfg.ModelName, metrics.StatusEmpty,
			c.Perf.QueueDuration, c.Perf.ContextDuration,
			c.Perf.LLMDuration, c.Perf.TotalDuration)
		metrics.IncrementCompletionRequests(h.cfg.ModelName, metrics.StatusEmpty)
		metrics.RecordCompletionTokens(h.cfg.ModelName, metrics.TokenTypeInput, c.Perf.PromptTokens)
		metrics.RecordCompletionTokens(h.cfg.ModelName, metrics.TokenTypeOutput, 0)
		return ErrorResponse(req, model.CompletionEmpty, c.Perf, dbg, fmt.Errorf("empty"))
	}

	// 记录成功指标
	metrics.RecordCompletionDuration(h.cfg.ModelName, metrics.StatusSuccess,
		c.Perf.QueueDuration, c.Perf.ContextDuration,
		c.Perf.LLMDuration, c.Perf.TotalDuration)
	metrics.IncrementCompletionRequests(h.cfg.ModelName, metrics.StatusSuccess)
	metrics.RecordCompletionTokens(h.cfg.ModelName, metrics.TokenTypeInput, c.Perf.PromptTokens)
	metrics.RecordCompletionTokens(h.cfg.ModelName, metrics.TokenTypeOutput, c.Perf.CompletionTokens)

	// 7. 构建响应
	return SuccessResponse(req, completionText, c.Perf, dbg)
}
