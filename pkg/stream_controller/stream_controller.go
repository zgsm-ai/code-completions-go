package stream_controller

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/config"
	"code-completion/pkg/model"
	"context"
	"fmt"
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
	req := sc.queues.AddRequest(ctx, input)
	pool := sc.pools.SelectIdlestPool(input.Model)
	if pool == nil {
		req.Input.SelectedModel = input.Model
		return completions.RejectRequest(req.Input, &req.Perf, model.StatusBusy, fmt.Errorf("model pool busy, cancel request"))
	}
	req.Input.SelectedModel = pool.cfg.ModelName
	defer func() {
		sc.queues.RemoveRequest(req)
	}()

	c := completions.NewCompletionContext(req.ctx, &req.Perf)
	rsp := input.Preprocess(c)
	if rsp != nil {
		return rsp
	}
	req.Perf.EnqueueTime = time.Now().Local()

	return sc.pools.WaitDoRequest(req)
}

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

func (sc *StreamController) GetDetails() map[string]interface{} {
	details := make(map[string]interface{})
	details["queues"] = sc.queues.GetDetails()
	details["pools"] = sc.pools.GetDetails()
	return details
}
