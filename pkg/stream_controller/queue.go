package stream_controller

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/config"
	"context"
	"sync"
	"time"

	"go.uber.org/zap"
)

//
//	等待队列: 所有来自客户端的请求先排队，等待调度到模型请求池
//

// 客户端
type CompletionClient struct {
	ClientID   string
	Latest     *ClientRequest
	LatestTime time.Time
}

// 等待队列管理器
type QueueManager struct {
	clients        map[string]*CompletionClient
	activeRequests int // 活跃请求数量统计
	mutex          sync.RWMutex
}

// 创建等待队列管理器
func NewQueueManager() *QueueManager {
	return &QueueManager{
		clients: make(map[string]*CompletionClient),
	}
}

// 添加请求到等待队列
func (m *QueueManager) AddRequest(ctx context.Context, input *completions.CompletionInput) *ClientRequest {
	m.mutex.Lock()
	defer m.mutex.Unlock()

	client, exists := m.clients[input.ClientID]
	if !exists {
		client = &CompletionClient{
			ClientID: input.ClientID,
		}
		m.clients[input.ClientID] = client
	}
	reqCtx, cancel := context.WithTimeout(ctx, config.Config.StreamController.CompletionTimeout)
	creq := &ClientRequest{
		Input:    input,
		Canceled: false,
		ctx:      reqCtx,
		cancel:   cancel,
		rspChan:  make(chan *completions.CompletionResponse, 1),
	}
	creq.Perf.ReceiveTime = time.Now().Local()

	// 增加活跃请求计数
	m.activeRequests++

	// 取消队列中所有现有请求
	if client.Latest != nil {
		m.cancelRequest(client.Latest)
		client.Latest = nil
	}
	client.Latest = creq

	zap.L().Debug("Add request to queue",
		zap.String("clientID", input.ClientID),
		zap.String("completionID", input.CompletionID),
		zap.Any("body", &input.CompletionRequest),
		zap.Any("headers", input.Headers),
		zap.Time("receiveTime", creq.Perf.ReceiveTime),
		zap.Int("activeRequests", m.activeRequests))
	return creq
}

func (m *QueueManager) RemoveRequest(req *ClientRequest) {
	m.mutex.Lock()
	defer m.mutex.Unlock()

	// 减少活跃请求计数
	if m.activeRequests > 0 {
		m.activeRequests--
	}

	queue, exists := m.clients[req.Input.ClientID]
	if !exists {
		return
	}

	// 如果移除的是当前请求，清空当前请求
	if queue.Latest == req {
		queue.Latest = nil
	}

	zap.L().Debug("Remove request from queue",
		zap.String("clientID", req.Input.ClientID),
		zap.String("completionID", req.Input.CompletionID),
		zap.Duration("duration", time.Since(req.Perf.ReceiveTime)),
		zap.Int("activeRequests", m.activeRequests))
}

// 取消现有请求
func (m *QueueManager) cancelRequest(req *ClientRequest) {
	zap.L().Debug("Cancel request",
		zap.String("clientID", req.Input.ClientID),
		zap.String("completionID", req.Input.CompletionID))
	// 这里可以添加更多的取消逻辑，比如通知模型池取消请求
	if req.cancel != nil {
		req.cancel()
	}
	req.Canceled = true
}

// 获取统计信息
func (m *QueueManager) GetStats() map[string]interface{} {
	m.mutex.RLock()
	defer m.mutex.RUnlock()

	activatedClient := 0
	for _, client := range m.clients {
		if client.Latest != nil {
			activatedClient++
		}
	}
	stats := make(map[string]interface{})
	stats["requests"] = map[string]interface{}{
		"total": m.activeRequests,
	}
	stats["clients"] = map[string]interface{}{
		"activated": activatedClient,
		"idled":     len(m.clients) - activatedClient,
		"total":     len(m.clients),
	}

	return stats
}

// 清理过期的队列
func (m *QueueManager) Cleanup() {
	m.mutex.Lock()
	defer m.mutex.Unlock()

	// 清理长时间没有活动的客户端
	for _, client := range m.clients {
		if client.Latest == nil {
			// 没有活跃请求的客户端可以考虑移除
			// 这里可以根据实际需求添加更复杂的逻辑
			// 例如：基于最后活动时间的老化机制
		}
	}
}

// func (m *QueueManager) FindEarliestRequest() *ClientRequest {
// 	m.mutex.RLock()
// 	defer m.mutex.RUnlock()

// 	return m.global.FindEarliestRequest()
// }

// 获取活跃请求数量（用于并发连接总数指标）
func (m *QueueManager) GetGlobalQueueLength() int {
	m.mutex.RLock()
	defer m.mutex.RUnlock()
	return m.activeRequests
}
