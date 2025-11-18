package completions

import (
	"code-completion/pkg/codebase_context"
	"code-completion/pkg/config"
	"code-completion/pkg/model"
	"net/http"
	"strings"
	"time"
)

type CompletionInput struct {
	CompletionRequest               //原始请求中的BODY
	Headers           http.Header   //原始请求中的头部
	Processed         PromptOptions //加工过的提示词
	SelectedModel     string        //根据服务端设置调整过的模型
}

var contextClient *codebase_context.ContextClient

// 处理补全请求
func (in *CompletionInput) Preprocess(c *CompletionContext) *CompletionResponse {
	// 0. 补全拒绝规则链处理
	err := NewFilterChain(&config.Config.CompletionsConfig).Handle(&in.CompletionRequest)
	if err != nil {
		return CancelRequest(in.CompletionID, in.SelectedModel, c.Perf, model.StatusRejected, err)
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
	if in.Processed.CodeContext != "" {
		return
	}
	if contextClient == nil {
		contextClient = codebase_context.NewContextClient()
	}
	in.Processed.CodeContext = contextClient.GetContext(
		c.Ctx,
		in.ClientID,
		in.ProjectPath,
		in.FileProjectPath,
		in.Processed.Prefix,
		in.Processed.Suffix,
		in.ImportContent,
		in.Headers,
	)
	c.Perf.ContextDuration = time.Since(c.Perf.ReceiveTime).Milliseconds()
}

/**
 * 解析提示词
 */
func (in *CompletionInput) GetPrompts() {
	req := &in.CompletionRequest
	if req.PromptOptions != nil {
		in.Processed = *req.PromptOptions
	} else {
		// 简单的提示词解析逻辑，参考Python代码
		// 可以根据FIM_INDICATOR分割prompt来获取prefix和suffix
		in.Processed.Prefix = req.Prompt
	}

	// 如果linePrefix为空，从prefix中提取
	if in.Processed.CursorLinePrefix == "" && in.Processed.Prefix != "" {
		lines := strings.Split(in.Processed.Prefix, "\n")
		if len(lines) > 0 {
			in.Processed.CursorLinePrefix = lines[len(lines)-1]
		}
	}

	// 如果lineSuffix为空，从suffix中提取
	if in.Processed.CursorLineSuffix == "" && in.Processed.Suffix != "" {
		lines := strings.Split(in.Processed.Suffix, "\n")
		if len(lines) > 0 {
			in.Processed.CursorLineSuffix = lines[0]
			if len(lines) > 1 {
				in.Processed.CursorLineSuffix += "\n"
			}
		}
	}
}
