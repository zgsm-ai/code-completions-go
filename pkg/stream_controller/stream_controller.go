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
	var perf completions.CompletionPerformance
	perf.ReceiveTime = time.Now().Local()
	// 如果无法获取到clientID和completionID，拒掉
	if input.ClientID == "" || input.CompletionID == "" {
		return completions.CancelRequest(input.CompletionID, input.Model, &perf, model.StatusRejected, fmt.Errorf("missing client id or completion id"))
	}
	//	预选模型池
	pool := sc.pools.SelectIdlestPool(input.Model)
	if pool == nil {
		return completions.CancelRequest(input.CompletionID, input.Model, &perf, model.StatusBusy, fmt.Errorf("model pool busy, cancel request"))
	}
	input.SelectedModel = pool.cfg.ModelName

	//	上下文预处理
	c := completions.NewCompletionContext(ctx, &perf)
	rsp := input.Preprocess(c)
	if rsp != nil {
		return rsp
	}
	//	请求数据针对模型进行适应性改造
	handler := completions.NewCompletionHandler(pool.llm)
	para := handler.Adapt(input)

	// 将请求添加到客户端队列，获取包含响应通道的ClientRequest
	req := sc.queues.AddRequest(ctx, para, &perf)
	defer func() {
		sc.queues.RemoveRequest(req)
	}()
	return sc.pools.WaitDoRequest(req)
}

func (sc *StreamController) ProcessCompletionV2(ctx context.Context, para *model.CompletionParameter) *completions.CompletionResponse {
	var perf completions.CompletionPerformance
	perf.ReceiveTime = time.Now().Local()

	req := sc.queues.AddRequest(ctx, para, &perf)
	defer func() {
		sc.queues.RemoveRequest(req)
	}()
	return sc.pools.WaitDoRequest(req)
}

func (sc *StreamController) ProcessCompletionOpenAI(ctx context.Context, r *model.CompletionRequest) *completions.CompletionResponse {
	var perf completions.CompletionPerformance
	perf.ReceiveTime = time.Now().Local()

	pool := sc.pools.findIdlestPool(sc.pools.all)
	if pool == nil {
		return completions.CancelRequest("", r.Model, &perf, model.StatusBusy, fmt.Errorf("model pool busy, cancel request"))
	}
	handler := completions.NewCompletionHandler(pool.llm)
	c := completions.NewCompletionContext(ctx, &perf)
	return handler.HandleCompletionOpenAI(c, r)
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
