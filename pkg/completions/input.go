package completions

import (
	"code-completion/pkg/codebase_context"
	"code-completion/pkg/config"
	"net/http"
	"strings"
	"time"
)

type CompletionInput struct {
	CompletionRequest
	Headers   http.Header
	PromptExt PromptOptions
}

var contextClient *codebase_context.ContextClient

// 处理补全请求
func (in *CompletionInput) Preprocess(c *CompletionContext) *CompletionResponse {
	// 0. 补全拒绝规则链处理
	err := NewFilterChain(&config.Config.CompletionsConfig).Handle(&in.CompletionRequest)
	if err != nil {
		c.Perf.TotalDuration = time.Since(c.Perf.ReceiveTime)
		return RejectRequest(&in.CompletionRequest, c.Perf, err)
	}

	// 1. 解析请求参数
	in.GetPrompts()

	// 2. 获取上下文信息
	in.GetContext(c)

	return nil
}

/**
 * 获取上下文信息
 */
func (in *CompletionInput) GetContext(c *CompletionContext) {
	if in.PromptExt.CodeContext != "" {
		return
	}
	if contextClient == nil {
		contextClient = codebase_context.NewContextClient()
	}
	in.PromptExt.CodeContext = contextClient.GetContext(
		c.Ctx,
		in.ClientID,
		in.ProjectPath,
		in.FileProjectPath,
		in.PromptExt.Prefix,
		in.PromptExt.Suffix,
		in.ImportContent,
		in.Headers,
	)
	c.Perf.ContextDuration = time.Since(c.Perf.ReceiveTime)
}

/**
 * 解析提示词
 */
func (in *CompletionInput) GetPrompts() {
	req := &in.CompletionRequest
	if req.PromptOptions != nil {
		in.PromptExt = *req.PromptOptions
	} else {
		// 简单的提示词解析逻辑，参考Python代码
		// 可以根据FIM_INDICATOR分割prompt来获取prefix和suffix
		in.PromptExt.Prefix = req.Prompt
	}

	// 如果linePrefix为空，从prefix中提取
	if in.PromptExt.CursorLinePrefix == "" && in.PromptExt.Prefix != "" {
		lines := strings.Split(in.PromptExt.Prefix, "\n")
		if len(lines) > 0 {
			in.PromptExt.CursorLinePrefix = lines[len(lines)-1]
		}
	}

	// 如果lineSuffix为空，从suffix中提取
	if in.PromptExt.CursorLineSuffix == "" && in.PromptExt.Suffix != "" {
		lines := strings.Split(in.PromptExt.Suffix, "\n")
		if len(lines) > 0 {
			in.PromptExt.CursorLineSuffix = lines[0]
			if len(lines) > 1 {
				in.PromptExt.CursorLineSuffix += "\n"
			}
		}
	}
}
