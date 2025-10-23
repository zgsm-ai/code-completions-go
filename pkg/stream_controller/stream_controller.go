package stream_controller

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/config"
	"context"
	"fmt"
	"net/http"
	"time"

	"go.uber.org/zap"
)

// Controller 全局流控管理器
var Controller *StreamController

// StreamController 流控管理器,对补全模型的访问做流控，防止补全模型失去响应
type StreamController struct {
	queues *QueueManager //请求等待队列管理（在等待调度到模型请求池）
	pools  *PoolManager  //模型请求池管理（正在调用模型的请求）
}

func NewStreamController() *StreamController {
	return &StreamController{
		queues: NewQueueManager(),
		pools:  NewPoolManager(),
	}
}

func (sc *StreamController) Init() {
	sc.pools.Init()

	var maintainInterval time.Duration
	maintainInterval = time.Duration(300) * time.Second // 默认清理间隔（秒）
	if config.Config.StreamController.MaintainInterval > 0 {
		maintainInterval = config.Config.StreamController.MaintainInterval
	}
	sc.StartMaintainRoutine(maintainInterval)

	zap.L().Info("Initialize queue configuration",
		zap.Duration("maintainInterval", maintainInterval))
}

// 处理补全请求
func (sc *StreamController) ProcessCompletionRequest(ctx context.Context, req *completions.CompletionRequest, headers http.Header) (*completions.CompletionResponse, error) {
	if req.ClientID == "" { // 如果无法获取客户端ID，使用默认处理方式
		return sc.processWithoutStreamControl(ctx, req, headers)
	}

	// 将请求添加到客户端队列，获取包含响应通道的ClientRequest
	clientReq := sc.queues.AddRequest(req, ctx, headers)
	defer sc.queues.RemoveRequest(clientReq)

	// 启动异步处理协程，选中等待队列的某个请求进行处理
	go sc.scheduleRequest()

	// 等待响应或错误
	select {
	case response := <-clientReq.ResponseChan:
		return response, nil
	case err := <-clientReq.ErrorChan:
		return nil, err
	case <-clientReq.ctx.Done():
		zap.L().Warn("ProcessCompletionRequest context canceled",
			zap.String("clientID", req.ClientID),
			zap.String("completionID", req.CompletionID),
			zap.Error(clientReq.ctx.Err()))
		return nil, clientReq.ctx.Err()
	}
}

// 调度请求，从所有客户端队列中选择最早的请求进行处理
func (sc *StreamController) scheduleRequest() error {
	// 1. 从所有客户端队列中找到最早的请求
	req := sc.queues.FindEarliestRequest()
	if req == nil {
		return fmt.Errorf("no available requests to process")
	}

	// 2. 将请求添加到模型请求池，执行模型调用
	response, err := sc.pools.DoRequest(req)

	// 3. 将模型的响应或错误发送到对应的通道
	if err != nil {
		req.ErrorChan <- err
	} else {
		req.ResponseChan <- response
	}
	return err
}

// 不使用流控的处理方式
func (sc *StreamController) processWithoutStreamControl(ctx context.Context, req *completions.CompletionRequest, headers http.Header) (*completions.CompletionResponse, error) {
	handler := completions.NewCompletionHandler()
	if handler == nil {
		return nil, fmt.Errorf("failed to create completion handler")
	}

	return handler.HandleCompletion(ctx, req, headers)
}

// 启动定期维护的协程
func (sc *StreamController) StartMaintainRoutine(interval time.Duration) {
	go func() {
		ticker := time.NewTicker(interval)
		defer ticker.Stop()

		for range ticker.C {
			sc.queues.Cleanup()
			zap.L().Info("StreamController maintain", zap.Any("stats", sc.GetStats()))
		}
	}()

	zap.L().Info("Start maintain routine", zap.Duration("interval", interval))
}

// 获取流控统计信息
func (sc *StreamController) GetStats() map[string]interface{} {
	stats := make(map[string]interface{})
	stats["queues"] = sc.queues.GetStats()
	stats["pools"] = sc.pools.GetStats()
	return stats
}
