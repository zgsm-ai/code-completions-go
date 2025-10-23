package stream_controller

import (
	"code-completion/pkg/completions"
	"context"
	"net/http"
	"time"
)

// 客户端请求包装器
type ClientRequest struct {
	Request      *completions.CompletionRequest       // 客户端发出的补全请求参数
	ReceiveTime  time.Time                            // 请求接收时间
	Pool         *ModelPool                           // 如果请求开始调用模型，则Pool指向正在处理请求的模型的请求池。
	headers      http.Header                          // 客户端发出的补全请求的请求头
	Canceled     bool                                 // 请求是否被取消
	ctx          context.Context                      //请求关联的协程上下文
	cancel       context.CancelFunc                   // 可以取消执行请求的协程
	ResponseChan chan *completions.CompletionResponse // 响应通道
	ErrorChan    chan error                           // 错误通道
}
