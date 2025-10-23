package stream_controller

// GlobalQueue 总队列管理器
type GlobalQueue struct {
	Requests []*ClientRequest // 总队列，按接收时间排序
}

func NewGlobalQueue() *GlobalQueue {
	return &GlobalQueue{
		Requests: make([]*ClientRequest, 0),
	}
}

func (gq *GlobalQueue) AddRequest(clientReq *ClientRequest) {
	gq.Requests = append(gq.Requests, clientReq)
}

func (gq *GlobalQueue) RemoveRequest(clientReq *ClientRequest) {
	for i, req := range gq.Requests {
		if req == clientReq {
			gq.Requests = append(gq.Requests[:i], gq.Requests[i+1:]...)
			break
		}
	}
}

// 从总队列中找到第一个未取消的请求
func (gq *GlobalQueue) FindEarliestRequest() *ClientRequest {
	for _, req := range gq.Requests {
		if !req.Canceled {
			return req
		}
	}
	return nil
}

// 清理总队列中的已取消请求
func (gq *GlobalQueue) Cleanup() int {
	cleanedCount := 0
	newQueue := make([]*ClientRequest, 0, len(gq.Requests))

	for _, req := range gq.Requests {
		if !req.Canceled {
			newQueue = append(newQueue, req)
		} else {
			cleanedCount++
		}
	}
	gq.Requests = newQueue
	return cleanedCount
}
