package stream_controller

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/config"
	"sync"
	"time"

	"go.uber.org/zap"
)

//
//	每个模型建立一个请求池，管理正在调用该模型的补全请求
//

// ModelPool 模型请求池
type ModelPool struct {
	Model     string
	cfg       *config.ModelConfig
	requests  map[string]*ClientRequest
	mutex     sync.RWMutex
	semaphore chan struct{}
}

// PoolManager 模型请求池管理器
type PoolManager struct {
	pools map[string]*ModelPool
	def   *ModelPool
}

// NewPoolManager 创建模型请求池管理器
func NewPoolManager() *PoolManager {
	return &PoolManager{
		pools: make(map[string]*ModelPool),
	}
}

func (m *PoolManager) Init() {
	for i, model := range config.Config.Models {
		modelName := model.ModelName
		if modelName == "" {
			modelName = "default"
		}
		p := m.initPool(modelName, &config.Config.Models[i])
		if m.def == nil {
			m.def = p
		}
	}
	if m.def == nil {
		zap.L().Error("Initialize model error, 'models' is missing",
			zap.Int("modelCount", len(config.Config.Models)))
		panic("config missing 'models'")
	}
}

// initPool 初始化模型请求池
func (m *PoolManager) initPool(model string, cfg *config.ModelConfig) *ModelPool {
	if existingPool, exists := m.pools[model]; exists {
		close(existingPool.semaphore)
	}

	pool := &ModelPool{
		Model:     model,
		cfg:       cfg,
		requests:  make(map[string]*ClientRequest),
		semaphore: make(chan struct{}, cfg.MaxConcurrent),
	}

	// 初始化信号量
	for i := 0; i < cfg.MaxConcurrent; i++ {
		pool.semaphore <- struct{}{}
	}

	m.pools[model] = pool

	zap.L().Info("Initialize model pool",
		zap.String("model", model),
		zap.Int("maxConcurrent", cfg.MaxConcurrent))
	return pool
}

// 等待模型池空闲处理请求
func (m *PoolManager) DoRequest(req *ClientRequest) *completions.CompletionResponse {
	pool, exists := m.pools[req.Request.Model]
	if !exists {
		pool = m.def
	}
	// 等待信号量（即有模型完成补全请求，可以执行新请求）或上下文取消
	select {
	case <-pool.semaphore: // 获取到信号量，处理请求
		return m.processRequest(pool, req)
	case <-req.ctx.Done(): // 请求被取消
		zap.L().Warn("Request canceled, Semaphore wait timeout",
			zap.String("model", req.Request.Model),
			zap.String("clientID", req.Request.ClientID),
			zap.String("completionID", req.Request.CompletionID),
			zap.Error(req.ctx.Err()))
		return completions.CancelRequest(req.Request, req.ctx.Err())
	}
}

// 处理请求
func (m *PoolManager) processRequest(pool *ModelPool, req *ClientRequest) *completions.CompletionResponse {
	startTime := time.Now()

	// 将请求添加到活跃请求列表
	pool.mutex.Lock()
	pool.requests[req.Request.CompletionID] = req
	req.Pool = pool
	pool.mutex.Unlock()

	zap.L().Debug("Start processing model request",
		zap.String("model", pool.Model),
		zap.String("clientID", req.Request.ClientID),
		zap.String("completionID", req.Request.CompletionID),
		zap.Int("requests", len(pool.requests)))

	// 处理完成后释放信号量
	defer func() {
		pool.mutex.Lock()
		delete(pool.requests, req.Request.CompletionID)
		pool.mutex.Unlock()
		req.Pool = nil
		select {
		case pool.semaphore <- struct{}{}: // 成功释放信号量，表示又有一个空位，可调度补全请求
		default: // 信号量已满，不应该发生
			zap.L().Error("Semaphore release failed",
				zap.String("model", pool.Model),
				zap.String("clientID", req.Request.ClientID),
				zap.String("completionID", req.Request.CompletionID))
		}
		zap.L().Debug("Completed processing model request",
			zap.String("model", pool.Model),
			zap.String("clientID", req.Request.ClientID),
			zap.String("completionID", req.Request.CompletionID),
			zap.Duration("duration", time.Since(startTime)))
	}()
	// 使用原有的补全处理器处理请求
	handler := completions.NewCompletionHandler()
	c := completions.NewCompletionContext(req.ctx, &req.Perf)
	// 处理请求
	return handler.HandleCompletion(c, req.Request, req.headers)
}

// 获取统计信息
func (m *PoolManager) GetStats() map[string]interface{} {
	stats := make(map[string]interface{})
	stats["total_pools"] = len(m.pools)

	poolDetails := make(map[string]interface{})
	for model, pool := range m.pools {
		pool.mutex.RLock()
		poolDetails[model] = map[string]interface{}{
			"max_concurrent":  pool.cfg.MaxConcurrent,
			"requests":        len(pool.requests),
			"available_slots": len(pool.semaphore),
		}
		pool.mutex.RUnlock()
	}
	stats["pools"] = poolDetails
	return stats
}
