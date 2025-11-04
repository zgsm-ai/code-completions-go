package stream_controller

import (
	"code-completion/pkg/completions"
	"context"
)

// 客户端请求包装器
type ClientRequest struct {
	Input    *completions.CompletionInput         // 客户端发出的补全请求参数
	Perf     completions.CompletionPerformance    // 性能统计
	Pool     *ModelPool                           // 如果请求开始调用模型，则Pool指向正在处理请求的模型的请求池。
	Canceled bool                                 // 请求是否被取消
	ctx      context.Context                      // 请求关联的协程上下文
	cancel   context.CancelFunc                   // 可以取消执行请求的协程
	rspChan  chan *completions.CompletionResponse // 响应通道
}
