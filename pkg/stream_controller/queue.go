package stream_controller

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/config"
	"context"
	"net/http"
	"sync"
	"time"

	"go.uber.org/zap"
)

//
//	等待队列: 所有来自客户端的请求先排队，等待调度到模型请求池
//

// ClientQueue 客户端
type ClientQueue struct {
	ClientID   string
	CurrentReq *ClientRequest
}

// QueueManager 等待队列管理器
type QueueManager struct {
	queues map[string]*ClientQueue
	global *GlobalQueue
	mutex  sync.RWMutex
}

// 创建等待队列管理器
func NewQueueManager() *QueueManager {
	return &QueueManager{
		queues: make(map[string]*ClientQueue),
		global: NewGlobalQueue(),
	}
}

// 添加请求到等待队列
func (m *QueueManager) AddRequest(req *completions.CompletionRequest, ctx context.Context, headers http.Header) *ClientRequest {
	m.mutex.Lock()
	defer m.mutex.Unlock()

	queue, exists := m.queues[req.ClientID]
	if !exists {
		queue = &ClientQueue{
			ClientID: req.ClientID,
		}
		m.queues[req.ClientID] = queue
	}
	reqCtx, cancel := context.WithTimeout(ctx, config.Config.StreamController.CompletionTimeout)
	// 创建客户端请求包装器
	clientReq := &ClientRequest{
		Request:      req,
		ReceiveTime:  time.Now(),
		Canceled:     false,
		ctx:          reqCtx,
		cancel:       cancel,
		headers:      headers,
		ResponseChan: make(chan *completions.CompletionResponse, 1),
	}

	m.global.AddRequest(clientReq)

	// 取消队列中所有现有请求
	if queue.CurrentReq != nil {
		m.cancelExistingRequest(queue.CurrentReq)
		queue.CurrentReq = nil
	}
	queue.CurrentReq = clientReq

	zap.L().Debug("Add request to queue",
		zap.String("clientID", req.ClientID),
		zap.String("completionID", req.CompletionID),
		zap.Any("body", req),
		zap.Any("headers", headers),
		zap.Time("receiveTime", clientReq.ReceiveTime),
		zap.Int("queueSize", len(m.global.Requests)))
	return clientReq
}

func (m *QueueManager) RemoveRequest(req *ClientRequest) {
	m.mutex.Lock()
	defer m.mutex.Unlock()

	// 从总队列中移除请求
	m.global.RemoveRequest(req)

	queue, exists := m.queues[req.Request.ClientID]
	if !exists {
		return
	}

	// 如果移除的是当前请求，清空当前请求
	if queue.CurrentReq == req {
		queue.CurrentReq = nil
	}

	zap.L().Debug("Remove request from queue",
		zap.String("clientID", req.Request.ClientID),
		zap.String("completionID", req.Request.CompletionID),
		zap.Duration("duration", time.Since(req.ReceiveTime)),
		zap.Int("queueSize", len(m.global.Requests)))
}

// cancelExistingRequest 取消现有请求
func (m *QueueManager) cancelExistingRequest(req *ClientRequest) {
	zap.L().Warn("Cancel existing request",
		zap.String("clientID", req.Request.ClientID),
		zap.String("completionID", req.Request.CompletionID))
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

	stats := make(map[string]interface{})
	stats["total_queues"] = len(m.queues)

	canceledCount := 0
	for _, req := range m.global.Requests {
		if req.Canceled {
			canceledCount++
		}
	}
	stats["total_requests"] = len(m.global.Requests)
	stats["canceled_requests"] = canceledCount
	stats["active_requests"] = len(m.global.Requests) - canceledCount

	queueDetails := make(map[string]interface{})
	for clientID, queue := range m.queues {
		queueDetails[clientID] = map[string]interface{}{
			"current_req": queue.CurrentReq != nil,
		}
	}

	stats["queues"] = queueDetails

	return stats
}

// 清理过期的队列
func (m *QueueManager) Cleanup() {
	m.mutex.Lock()
	defer m.mutex.Unlock()

	// 清理总队列中的已取消请求
	m.global.Cleanup()
	// 这里可以添加清理逻辑，比如删除长时间没有活动的队列
	// 暂时简单实现，可以后续扩展
}

func (m *QueueManager) FindEarliestRequest() *ClientRequest {
	m.mutex.RLock()
	defer m.mutex.RUnlock()

	return m.global.FindEarliestRequest()
}
