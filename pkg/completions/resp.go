package completions

import (
	"code-completions/pkg/model"
	"time"
)

// CompletionResponse 补全响应结构
type CompletionResponse struct {
	ID               string                 `json:"id"`
	Model            string                 `json:"model"`
	Object           string                 `json:"object"`
	RequestTime      time.Time              `json:"request_time"`
	Choices          []CompletionChoice     `json:"choices"`
	ModelChoices     []CompletionChoice     `json:"model_choices"`
	ModelStartTime   *time.Time             `json:"model_start_time"` // may null
	ModelEndTime     *time.Time             `json:"model_end_time"`
	ModelCostTime    int                    `json:"model_cost_time"` // ms
	CompletionTokens int                    `json:"completion_tokens"`
	PromptTokens     int                    `json:"prompt_tokens"`
	Prompt           string                 `json:"prompt"`
	Status           model.CompletionStatus `json:"status"`
}

// CompletionChoice 补全选择
type CompletionChoice struct {
	Text string `json:"text"`
}

func ErrorResponse(id string, modelName string, ErrorCode model.CompletionStatus, reqTime time.Time,
	Prompt string, PromptTokens int,
	ModelStartTime *time.Time, ModelEndTime *time.Time, ModelCostTime int, ModelChoices []CompletionChoice) *CompletionResponse {
	return &CompletionResponse{
		ID:             id,
		Model:          modelName,
		Object:         "text_completion",
		RequestTime:    reqTime, // 可以使用时间戳
		Prompt:         Prompt,
		PromptTokens:   PromptTokens,
		ModelStartTime: ModelStartTime,
		ModelEndTime:   ModelEndTime,
		ModelCostTime:  ModelCostTime,
		Choices:        []CompletionChoice{{Text: ""}}, // 使用后置处理后的补全结果
		ModelChoices:   ModelChoices,

		CompletionTokens: 0,
		Status:           ErrorCode,
	}
}

func SuccessResponse(id string, modelName string, completionText string, reqTime time.Time,
	Prompt string, PromptTokens int,
	ModelStartTime *time.Time, ModelEndTime *time.Time, ModelCostTime int, ModelChoices []CompletionChoice) *CompletionResponse {
	return &CompletionResponse{
		ID:               id,
		Model:            modelName,
		Object:           "text_completion",
		RequestTime:      reqTime, // 可以使用时间戳
		Prompt:           Prompt,
		PromptTokens:     PromptTokens,
		ModelStartTime:   ModelStartTime,
		ModelEndTime:     ModelEndTime,
		ModelCostTime:    ModelCostTime,
		Choices:          []CompletionChoice{{Text: completionText}}, // 使用后置处理后的补全结果
		ModelChoices:     ModelChoices,
		CompletionTokens: 0,
		Status:           model.CompletionSucess,
	}
}
