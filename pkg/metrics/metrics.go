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
			Buckets: []float64{10, 20, 35, 50, 100, 200, 300, 500, 1000, 2000, 3000, 4000, 5000},
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

	// 瞬时值指标：当前各模型池并发的连接总数
	completionConcurrent = promauto.NewGauge(
		prometheus.GaugeOpts{
			Name: "completion_concurrent",
			Help: "Current total number of concurrent connections across all model pools",
		},
	)

	// 瞬时值指标：各模型池并发的连接数（带model标签）
	completionConcurrentByModel = promauto.NewGaugeVec(
		prometheus.GaugeOpts{
			Name: "completion_concurrent_by_model",
			Help: "Current number of concurrent connections per model pool",
		},
		[]string{"model"},
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
	StatusRejected    Status = "rejected"
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

// 更新当前各模型池并发的连接总数
func UpdateCompletionConcurrent(count int) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	completionConcurrent.Set(float64(count))
}

// 更新指定模型池的并发连接数
func UpdateCompletionConcurrentByModel(model string, count int) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	completionConcurrentByModel.WithLabelValues(model).Set(float64(count))
}

// 返回Prometheus指标数据的HTTP处理器
func GetMetricsHandler() http.Handler {
	return promhttp.Handler()
}
