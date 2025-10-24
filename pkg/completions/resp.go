package completions

import (
	"code-completion/pkg/model"
	"context"
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
	ReceiveTime      time.Time //收到请求的时间
	StartTime        time.Time //排队完毕，开始处理的时间
	ModelStartTime   time.Time //请求模型的开始时间
	ModelEndTime     time.Time //请求模型的结束时间
	FulledDuration   int
	ModelDuration    int
	PromptTokens     int
	CompletionTokens int
}

// CompletionResponse 补全响应结构
type CompletionResponse struct {
	ID      string                 `json:"id"`
	Model   string                 `json:"model"`
	Object  string                 `json:"object"`
	Choices []CompletionChoice     `json:"choices"`
	Created int                    `json:"created"`
	Status  model.CompletionStatus `json:"status"`
	Usage   CompletionUsage        `json:"usage"`
	Origin  map[string]interface{} `json:"origin,omitempty"`
	Prompt  string                 `json:"prompt"`

	RequestTime      time.Time          `json:"request_time"`
	ModelChoices     []CompletionChoice `json:"model_choices"`
	ModelStartTime   *time.Time         `json:"model_start_time"` // may null
	ModelEndTime     *time.Time         `json:"model_end_time"`
	ModelCostTime    int                `json:"model_cost_time"` // ms
	CompletionTokens int                `json:"completion_tokens"`
	PromptTokens     int                `json:"prompt_tokens"`
}

func ErrorResponse(req *CompletionRequest, ErrorCode model.CompletionStatus, perf *CompletionPerformance,
	Prompt string, ModelChoices []CompletionChoice) *CompletionResponse {
	return &CompletionResponse{
		ID:               req.CompletionID,
		Model:            req.Model,
		Object:           "text_completion",
		RequestTime:      perf.ReceiveTime, // 可以使用时间戳
		Prompt:           Prompt,
		PromptTokens:     perf.PromptTokens,
		ModelStartTime:   &perf.ModelStartTime,
		ModelEndTime:     &perf.ModelEndTime,
		ModelCostTime:    perf.ModelDuration,
		Choices:          []CompletionChoice{{Text: ""}}, // 使用后置处理后的补全结果
		ModelChoices:     ModelChoices,
		CompletionTokens: 0,
		Status:           ErrorCode,
	}
}

func SuccessResponse(req *CompletionRequest, completionText string, perf *CompletionPerformance,
	Prompt string, ModelChoices []CompletionChoice) *CompletionResponse {
	return &CompletionResponse{
		ID:               req.CompletionID,
		Model:            req.Model,
		Object:           "text_completion",
		RequestTime:      perf.ReceiveTime, // 可以使用时间戳
		Prompt:           Prompt,
		PromptTokens:     perf.PromptTokens,
		Choices:          []CompletionChoice{{Text: completionText}}, // 使用后置处理后的补全结果
		ModelStartTime:   ModelStartTime,
		ModelEndTime:     ModelEndTime,
		ModelCostTime:    ModelCostTime,
		ModelChoices:     ModelChoices,
		CompletionTokens: 0,
		Status:           model.CompletionSuccess,
	}
}

// 取消请求
func CancelRequest(req *CompletionRequest, err error) *CompletionResponse {
	status := model.CompletionTimeout
	if err.Error() == context.Canceled.Error() {
		status = model.CompletionCanceled
	}
	return &CompletionResponse{
		ID:      req.CompletionID,
		Model:   req.Model,
		Object:  "text_completion",
		Choices: []CompletionChoice{{Text: ""}},
		Status:  status,
	}
}

func RejectRequest(req *CompletionRequest) *CompletionResponse {
	return &CompletionResponse{
		ID:      req.CompletionID,
		Model:   req.Model,
		Object:  "text_completion",
		Choices: []CompletionChoice{{Text: ""}},
		Status:  model.CompletionRejected,
		Created: int(time.Now().Local().Local().Unix()),
	}
}
