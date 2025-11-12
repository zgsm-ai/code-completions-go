package stream_controller

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/config"
	"code-completion/pkg/metrics"
	"context"
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
func (sc *StreamController) ProcessCompletionRequest(ctx context.Context, input *completions.CompletionInput) *completions.CompletionResponse {
	if input.ClientID == "" { // 如果无法获取客户端ID，使用默认处理方式
		return sc.processWithoutStreamControl(ctx, input)
	}
	// 将请求添加到客户端队列，获取包含响应通道的ClientRequest
	creq := sc.queues.AddRequest(ctx, input)
	metrics.UpdateCompletionConcurrent(sc.queues.GetGlobalQueueLength())
	defer func() {
		sc.queues.RemoveRequest(creq)
		metrics.UpdateCompletionConcurrent(sc.queues.GetGlobalQueueLength())
	}()

	c := completions.NewCompletionContext(creq.ctx, &creq.Perf)
	rsp := input.Preprocess(c)
	if rsp != nil {
		return rsp
	}
	creq.Perf.EnqueueTime = time.Now().Local()

	return sc.pools.WaitDoRequest(creq)

	// // 等待响应或错误
	// select {
	// case rsp := <-creq.rspChan:
	// 	return rsp
	// case <-creq.ctx.Done():
	// 	zap.L().Debug("ProcessCompletionRequest context canceled",
	// 		zap.String("clientID", input.ClientID),
	// 		zap.String("completionID", input.CompletionID),
	// 		zap.Error(creq.ctx.Err()))
	// 	return completions.CancelRequest(&input.CompletionRequest, &creq.Perf, creq.ctx.Err())
	// }
}

// // 调度请求，从所有客户端队列中选择最早的请求进行处理
// func (sc *StreamController) scheduleRequest() error {
// 	// 1. 从所有客户端队列中找到最早的请求
// 	req := sc.queues.FindEarliestRequest()
// 	if req == nil {
// 		return fmt.Errorf("no available requests to process")
// 	}

// 	// 2. 将请求添加到模型请求池，执行模型调用
// 	rsp := sc.pools.WaitDoRequest(req)

// 	// 3. 将模型的响应或错误发送到对应的通道
// 	req.rspChan <- rsp
// 	return nil
// }

// 不使用流控的处理方式
func (sc *StreamController) processWithoutStreamControl(ctx context.Context, input *completions.CompletionInput) *completions.CompletionResponse {
	pool := sc.pools.findIdlestPool(sc.pools.all)
	handler := completions.NewCompletionHandler(pool.llm)
	perf := &completions.CompletionPerformance{
		ReceiveTime: time.Now().Local(),
	}
	c := completions.NewCompletionContext(ctx, perf)
	return handler.HandleCompletion(c, input)
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
