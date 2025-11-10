package metrics

import (
	"net/http"
	"sync"
	"time"

	"github.com/prometheus/client_golang/prometheus"
	"github.com/prometheus/client_golang/prometheus/promauto"
	"github.com/prometheus/client_golang/prometheus/promhttp"
)

var (
	completionDurations = promauto.NewHistogramVec(
		prometheus.HistogramOpts{
			Name:    "completion_durations",
			Help:    "Duration of each phase of completion requests in milliseconds",
			Buckets: []float64{50, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1200, 1500, 2000, 2500, 5000},
		},
		[]string{"model", "status", "phase"},
	)

	// Token数量分布指标 (Histogram)
	completionTokens = promauto.NewHistogramVec(
		prometheus.HistogramOpts{
			Name:    "completion_tokens",
			Help:    "Number of input/output tokens in completion requests",
			Buckets: []float64{10, 50, 100, 300, 500, 1000, 2000, 3000, 4000, 5000, 10000, 50000},
		},
		[]string{"model", "type"},
	)

	// 请求总数指标 (Counter)
	completionRequestsTotal = promauto.NewCounterVec(
		prometheus.CounterOpts{
			Name: "completion_requests_total",
			Help: "Total number of completion requests",
		},
		[]string{"model", "status"},
	)

	// 互斥锁，确保线程安全
	metricsMutex sync.Mutex
)

// Status 定义请求状态
type Status string

const (
	StatusSuccess     Status = "success"
	StatusModelError  Status = "modelError"
	StatusReqError    Status = "reqError"
	StatusServerError Status = "serverError"
	StatusEmpty       Status = "empty"
	StatusReject      Status = "rejected"
	StatusTimeout     Status = "timeout"
	StatusCanceled    Status = "canceled"
)

type CompletionPhase string

const (
	PhaseQueue   CompletionPhase = "queue"
	PhaseContext CompletionPhase = "context"
	PhaseLLM     CompletionPhase = "llm"
	PhaseTotal   CompletionPhase = "total"
)

// 定义token类型
type TokenType string

const (
	TokenTypeInput  TokenType = "input"
	TokenTypeOutput TokenType = "output"
)

// 记录补全各阶段耗时
func RecordCompletionDuration(model string, status Status, queue, context, llm, total time.Duration) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	completionDurations.WithLabelValues(model, string(status), "queue").Observe(float64(queue.Milliseconds()))
	completionDurations.WithLabelValues(model, string(status), "context").Observe(float64(context.Milliseconds()))
	completionDurations.WithLabelValues(model, string(status), "llm").Observe(float64(llm.Milliseconds()))
	completionDurations.WithLabelValues(model, string(status), "total").Observe(float64(total.Milliseconds()))
}

// 记录每次请求的输入和输出token数分布
func RecordCompletionTokens(model string, tokenType TokenType, tokenCount int) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	completionTokens.WithLabelValues(model, string(tokenType)).Observe(float64(tokenCount))
}

// 记录请求总数，用于计算QPS和错误率
func IncrementCompletionRequests(model string, status Status) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	completionRequestsTotal.WithLabelValues(model, string(status)).Inc()
}

// 返回Prometheus指标数据的HTTP处理器
func GetMetricsHandler() http.Handler {
	return promhttp.Handler()
}
