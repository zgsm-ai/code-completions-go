package metrics

import (
	"net/http"
	"sync"

	"github.com/prometheus/client_golang/prometheus"
	"github.com/prometheus/client_golang/prometheus/promauto"
	"github.com/prometheus/client_golang/prometheus/promhttp"
)

var (
	// 总耗时分布指标 (Histogram)
	completionTotalDurationSeconds = promauto.NewHistogramVec(
		prometheus.HistogramOpts{
			Name:    "completion_total_duration_milliseconds",
			Help:    "Total duration of completion requests in milliseconds",
			Buckets: []float64{10, 50, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 2500, 5000, 10000},
		},
		[]string{"model", "status"},
	)

	// 模型耗时分布指标 (Histogram)
	completionModelDurationSeconds = promauto.NewHistogramVec(
		prometheus.HistogramOpts{
			Name:    "completion_model_duration_milliseconds",
			Help:    "Model inference duration of completion requests in milliseconds",
			Buckets: []float64{10, 50, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1500, 2000, 2500, 5000, 10000},
		},
		[]string{"model", "status"},
	)

	// Token数量分布指标 (Histogram)
	completionTokens = promauto.NewHistogramVec(
		prometheus.HistogramOpts{
			Name:    "completion_tokens",
			Help:    "Number of input/output tokens in completion requests",
			Buckets: []float64{10, 50, 100, 300, 500, 1000, 2000, 3000, 4000, 5000, 10000, 50000},
		},
		[]string{"type", "model"},
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

// Phase 定义请求阶段
type Phase string

const (
	PhaseTotal   Phase = "total"
	PhaseContext Phase = "context"
	PhaseLLM     Phase = "llm"
)

// Status 定义请求状态
type Status string

const (
	StatusSucess       Status = "sucess"
	StatusModelError   Status = "modelError"
	StatusReqError     Status = "reqError"
	StatusEmpty        Status = "empty"
	StatusProcessEmpty Status = "ProcessEmpty"
	StatusServerError  Status = "serverError"
	StatusReject       Status = "reject"
	StatusTimeout      Status = "timeout"
)

// TokenType 定义token类型
type TokenType string

const (
	TokenTypeInput  TokenType = "input"
	TokenTypeOutput TokenType = "output"
)

// 记录请求总耗时分布
func RecordCompletionTotalDuration(model string, status Status, duration float64) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	completionTotalDurationSeconds.WithLabelValues(model, string(status)).Observe(duration)
}

// 记录模型推理耗时分布
func RecordCompletionModelDuration(model string, status Status, duration float64) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	completionModelDurationSeconds.WithLabelValues(model, string(status)).Observe(duration)
}

// 记录请求在不同阶段的耗时分布 (保持向后兼容)
func RecordCompletionDuration(phase Phase, model string, status Status, duration float64) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	switch phase {
	case PhaseTotal:
		completionTotalDurationSeconds.WithLabelValues(model, string(status)).Observe(duration)
	case PhaseLLM:
		completionModelDurationSeconds.WithLabelValues(model, string(status)).Observe(duration)
	}
}

// 记录每次请求的输入和输出token数分布
func RecordCompletionTokens(tokenType TokenType, model string, tokenCount int) {
	metricsMutex.Lock()
	defer metricsMutex.Unlock()

	completionTokens.WithLabelValues(string(tokenType), model).Observe(float64(tokenCount))
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
