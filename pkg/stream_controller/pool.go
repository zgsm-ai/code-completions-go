package stream_controller

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/config"
	"code-completion/pkg/model"
	"sync"
	"time"

	"go.uber.org/zap"
)

//
//	每个模型建立一个请求池，管理正在调用该模型的补全请求
//

// 模型请求池
type ModelPool struct {
	llm       model.LLM
	cfg       *config.ModelConfig
	requests  map[string]*ClientRequest
	mutex     sync.RWMutex
	semaphore chan struct{}
}

// 模型请求池管理器
type PoolManager struct {
	pools map[string][]*ModelPool
	all   []*ModelPool
}

// 创建模型请求池管理器
func NewPoolManager() *PoolManager {
	return &PoolManager{
		pools: make(map[string][]*ModelPool),
		all:   make([]*ModelPool, 0),
	}
}

func (m *PoolManager) Init() {
	for i, cfg := range config.Config.Models {
		modelName := cfg.ModelName
		if modelName == "" {
			modelName = "default"
		}
		m.initPool(modelName, model.GetModel(i), &config.Config.Models[i])
	}
	if len(m.all) == 0 {
		zap.L().Error("Initialize model error, 'models' is missing",
			zap.Int("modelCount", len(config.Config.Models)))
		panic("config missing 'models'")
	}
}

// initPool 初始化模型请求池
func (m *PoolManager) initPool(model string, llm model.LLM, cfg *config.ModelConfig) *ModelPool {
	pool := &ModelPool{
		cfg:       cfg,
		llm:       llm,
		requests:  make(map[string]*ClientRequest),
		semaphore: make(chan struct{}, cfg.MaxConcurrent),
	}
	m.all = append(m.all, pool)

	// 初始化信号量
	for i := 0; i < cfg.MaxConcurrent; i++ {
		pool.semaphore <- struct{}{}
	}

	// 将池添加到对应的模型名下
	if _, exists := m.pools[model]; !exists {
		m.pools[model] = make([]*ModelPool, 0)
	}
	m.pools[model] = append(m.pools[model], pool)

	// 为每个标签也添加相同的池
	for _, t := range cfg.Tags {
		if _, exists := m.pools[t]; !exists {
			m.pools[t] = make([]*ModelPool, 0)
		}
		m.pools[t] = append(m.pools[t], pool)
	}

	zap.L().Info("Initialize model pool",
		zap.String("model", model),
		zap.Int("maxConcurrent", cfg.MaxConcurrent))
	return pool
}

/**
* Find the model pool with the lowest load rate from a list of pools
* @param {[]*ModelPool} pools - List of model pools to search
* @returns {ModelPool} Returns the model pool with the lowest load rate
* @description
* - Iterates through the provided pools to find the one with the lowest load rate
* - Load rate is calculated as: active_requests / max_concurrent
* - If multiple pools have the same load rate, returns the first one found
* - If the list is empty, returns nil
* @example
* pool := manager.findLowestLoadPool(pools)
 */
func (m *PoolManager) findIdlestPool(pools []*ModelPool) *ModelPool {
	if len(pools) == 0 {
		return nil
	}

	lowestLoadRate := float64(1.0) // Initialize with maximum possible load rate
	var selectedPool *ModelPool

	for _, pool := range pools {
		pool.mutex.RLock()
		activeRequests := len(pool.requests)
		maxConcurrent := pool.cfg.MaxConcurrent
		pool.mutex.RUnlock()

		// Calculate load rate (0.0 to 1.0, where 0.0 is idle and 1.0 is fully loaded)
		var loadRate float64
		if maxConcurrent > 0 {
			loadRate = float64(activeRequests) / float64(maxConcurrent)
		} else {
			loadRate = 1.0 // Pool with 0 max concurrent is considered fully loaded
		}

		// Update selected pool if this one has a lower load rate
		if loadRate < lowestLoadRate {
			lowestLoadRate = loadRate
			selectedPool = pool
		}
	}
	if selectedPool == nil {
		selectedPool = pools[0]
	}
	return selectedPool
}

// 等待模型池空闲处理请求
func (m *PoolManager) WaitDoRequest(req *ClientRequest) *completions.CompletionResponse {
	var pool *ModelPool

	pools, exists := m.pools[req.Request.Model]
	if !exists || len(pools) == 0 {
		pool = m.findIdlestPool(m.all)
	} else {
		pool = m.findIdlestPool(pools)
	}

	// 等待信号量（即有模型完成补全请求，可以执行新请求）或上下文取消
	select {
	case <-pool.semaphore: // 获取到信号量，处理请求
		return m.doRequest(pool, req)
	case <-req.ctx.Done(): // 请求被取消
		zap.L().Debug("Request canceled, Semaphore wait timeout",
			zap.String("model", req.Request.Model),
			zap.String("clientID", req.Request.ClientID),
			zap.String("completionID", req.Request.CompletionID),
			zap.Error(req.ctx.Err()))
		return completions.CancelRequest(req.Request, &req.Perf, req.ctx.Err())
	}
}

// 执行请求，调用补全模型
func (m *PoolManager) doRequest(pool *ModelPool, req *ClientRequest) *completions.CompletionResponse {
	// 将请求添加到活跃请求列表
	pool.mutex.Lock()
	pool.requests[req.Request.CompletionID] = req
	req.Pool = pool
	pool.mutex.Unlock()

	zap.L().Debug("Start processing model request",
		zap.String("model", pool.cfg.ModelName),
		zap.String("clientID", req.Request.ClientID),
		zap.String("completionID", req.Request.CompletionID),
		zap.Int("requests", len(pool.requests)))

	// 使用原有的补全处理器处理请求
	handler := completions.NewCompletionHandler(pool.llm)
	c := completions.NewCompletionContext(req.ctx, &req.Perf)
	rsp := handler.HandleCompletion(c, req.Request, req.headers)

	pool.mutex.Lock()
	delete(pool.requests, req.Request.CompletionID)
	req.Pool = nil
	pool.mutex.Unlock()
	select {
	case pool.semaphore <- struct{}{}: // 成功释放信号量，表示又有一个空位，可调度补全请求
	default: // 信号量已满，不应该发生
		zap.L().Error("Semaphore release failed",
			zap.String("model", pool.cfg.ModelName), zap.Int("semaphore", len(pool.semaphore)))
	}
	zap.L().Debug("Completed processing model request",
		zap.String("model", pool.cfg.ModelName),
		zap.String("clientID", req.Request.ClientID),
		zap.String("completionID", req.Request.CompletionID),
		zap.Duration("duration", time.Since(req.Perf.ReceiveTime)))
	return rsp
}

// 获取统计信息
func (m *PoolManager) GetStats() map[string]interface{} {
	stats := make(map[string]interface{})

	// 计算总的模型池数量
	stats["count"] = len(m.all)
	poolDetails := make([]map[string]interface{}, 0)
	for _, pool := range m.all {
		pool.mutex.RLock()
		poolInfo := map[string]interface{}{
			"name":            pool.cfg.ModelName,
			"tags":            pool.cfg.Tags,
			"max_concurrent":  pool.cfg.MaxConcurrent,
			"requests":        len(pool.requests),
			"available_slots": len(pool.semaphore),
		}
		pool.mutex.RUnlock()
		poolDetails = append(poolDetails, poolInfo)
	}
	stats["pools"] = poolDetails
	return stats
}
