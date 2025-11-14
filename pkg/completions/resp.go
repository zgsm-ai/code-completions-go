package completions

import (
	"code-completion/pkg/metrics"
	"code-completion/pkg/model"
	"context"
	"fmt"
	"time"
)

type CompletionUsage struct {
	PromptTokens     int `json:"prompt_tokens"`
	CompletionTokens int `json:"completion_tokens"`
	TotalTokens      int `json:"total_tokens"`
}

// 补全选择
type CompletionChoice struct {
	Text string `json:"text"`
}

type CompletionPerformance struct {
	ReceiveTime      time.Time     `json:"receive_time"`     //收到请求的时间
	EnqueueTime      time.Time     `json:"-"`                //开始排队时间
	ContextDuration  time.Duration `json:"context_duration"` //获取上下文的时长
	QueueDuration    time.Duration `json:"queue_duration"`   //排队时长
	LLMDuration      time.Duration `json:"llm_duration"`     //调用大语言模型耗用的时长
	TotalDuration    time.Duration `json:"total_duration"`   //总时长
	PromptTokens     int           `json:"prompt_tokens"`
	CompletionTokens int           `json:"completion_tokens"`
	TotalTokens      int           `json:"total_tokens"`
}

// 补全响应结构
type CompletionResponse struct {
	ID      string                   `json:"id"`
	Model   string                   `json:"model"`
	Object  string                   `json:"object"`
	Choices []CompletionChoice       `json:"choices"`
	Created int                      `json:"created"`
	Usage   CompletionPerformance    `json:"usage"`
	Status  model.CompletionStatus   `json:"status"`
	Error   string                   `json:"error"`
	Verbose *model.CompletionVerbose `json:"verbose,omitempty"`
}

func Metrics(modelName string, status string, perf *CompletionPerformance) {
	metrics.RecordCompletionDuration(modelName, status,
		perf.QueueDuration, perf.ContextDuration, perf.LLMDuration, perf.TotalDuration)
	metrics.IncrementCompletionRequests(modelName, status)
	metrics.RecordCompletionTokens(modelName, metrics.TokenTypeInput, perf.PromptTokens)
	metrics.RecordCompletionTokens(modelName, metrics.TokenTypeOutput, perf.CompletionTokens)
}

func ErrorResponse(input *CompletionInput, status model.CompletionStatus,
	perf *CompletionPerformance, verbose *model.CompletionVerbose, err error) *CompletionResponse {
	if err == nil {
		err = fmt.Errorf("%s", string(status))
	}
	Metrics(input.SelectedModel, string(status), perf)
	return &CompletionResponse{
		ID:      input.CompletionID,
		Model:   input.SelectedModel,
		Object:  "text_completion",
		Choices: []CompletionChoice{{Text: ""}}, // 使用后置处理后的补全结果
		Created: int(perf.ReceiveTime.Unix()),
		Usage:   *perf,
		Status:  status,
		Error:   err.Error(),
		Verbose: verbose,
	}
}

func SuccessResponse(input *CompletionInput, completionText string, perf *CompletionPerformance,
	verbose *model.CompletionVerbose) *CompletionResponse {
	if !input.Verbose {
		verbose = nil
	}
	Metrics(input.SelectedModel, string(model.StatusSuccess), perf)
	return &CompletionResponse{
		ID:      input.CompletionID,
		Model:   input.SelectedModel,
		Object:  "text_completion",
		Choices: []CompletionChoice{{Text: completionText}}, // 使用后置处理后的补全结果
		Created: int(perf.ReceiveTime.Unix()),
		Usage:   *perf,
		Status:  model.StatusSuccess,
		Verbose: verbose,
	}
}

// 取消请求
func CancelRequest(input *CompletionInput, perf *CompletionPerformance, err error) *CompletionResponse {
	status := model.StatusTimeout
	if err.Error() == context.Canceled.Error() {
		status = model.StatusCanceled
	}
	perf.TotalDuration = time.Since(perf.ReceiveTime)
	Metrics(input.SelectedModel, string(status), perf)
	return &CompletionResponse{
		ID:      input.CompletionID,
		Model:   input.SelectedModel,
		Object:  "text_completion",
		Choices: []CompletionChoice{{Text: ""}},
		Created: int(perf.ReceiveTime.Unix()),
		Usage:   *perf,
		Status:  status,
		Error:   err.Error(),
	}
}

func RejectRequest(input *CompletionInput, perf *CompletionPerformance, status model.CompletionStatus, err error) *CompletionResponse {
	Metrics(input.SelectedModel, string(status), perf)
	return &CompletionResponse{
		ID:      input.CompletionID,
		Model:   input.SelectedModel,
		Object:  "text_completion",
		Choices: []CompletionChoice{{Text: ""}},
		Created: int(perf.ReceiveTime.Unix()),
		Usage:   *perf,
		Status:  status,
		Error:   err.Error(),
	}
}
