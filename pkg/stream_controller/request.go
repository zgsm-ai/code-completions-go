package stream_controller

import (
	"code-completion/pkg/completions"
	"context"
)

// 客户端请求包装器
type ClientRequest struct {
	Input    *completions.CompletionInput         // 客户端发出的补全请求参数
	Perf     completions.CompletionPerformance    // 性能统计
	Canceled bool                                 // 请求是否被取消
	ctx      context.Context                      // 请求关联的协程上下文
	cancel   context.CancelFunc                   // 可以取消执行请求的协程
	rspChan  chan *completions.CompletionResponse // 响应通道
}

func (r *ClientRequest) GetDetails() map[string]interface{} {
	return map[string]interface{}{
		"completion_id": r.Input.CompletionID,
		"client_id":     r.Input.ClientID,
		"model": map[string]interface{}{
			"origin":   r.Input.Model,
			"selected": r.Input.SelectedModel,
		},
		"prompt": len(r.Input.Prompt),
		"processed": map[string]interface{}{
			"prefix":      len(r.Input.Processed.Prefix),
			"suffix":      len(r.Input.Processed.Suffix),
			"context":     len(r.Input.Processed.CodeContext),
			"line_prefix": r.Input.Processed.CursorLinePrefix,
			"line_suffix": r.Input.Processed.CursorLineSuffix,
		},
		"performance": r.Perf,
		"canceled":    r.Canceled,
	}
}

func (r *ClientRequest) GetSummary() map[string]interface{} {
	return map[string]interface{}{
		"completion_id": r.Input.CompletionID,
		"client_id":     r.Input.ClientID,
		"prompt":        len(r.Input.Processed.Prefix) + len(r.Input.Processed.Suffix) + len(r.Input.Processed.CodeContext),
		"canceled":      r.Canceled,
	}
}
