package model

//
//	OpenAI v1/completions协议的请求和响应结构定义
//
//
// 请求体结构(参考：https://api-docs.deepseek.com/zh-cn/api/create-completion)
//
type CompletionRequest struct {
	Model            string   `json:"model"`
	Prompt           string   `json:"prompt"`
	MaxTokens        int      `json:"max_tokens,omitempty"`
	Temperature      float32  `json:"temperature,omitempty"`
	TopP             float32  `json:"top_p,omitempty"`
	FrequencyPenalty float32  `json:"frequency_penalty,omitemtpy"`
	PresencePenalty  float32  `json:"presence_penalty,omitemtpy"`
	Stop             []string `json:"stop,omitempty"`
	Stream           bool     `json:"stream,omitempty"`
	Echo             bool     `json:"echo,omitemtpy"`
	Suffix           string   `json:"suffix,omitempty"`
}

type CompletionChoice struct {
	Text         string      `json:"text"`
	Index        int         `json:"index"`
	Logprobs     interface{} `json:"logprobs,omitempty"`
	FinishReason string      `json:"finish_reason"`
}

type CompletionUsage struct {
	PromptTokens            int         `json:"prompt_tokens"`
	CompletionTokens        int         `json:"completion_tokens"`
	TotalTokens             int         `json:"total_tokens"`
	PromptCacheHitTokens    int         `json:"prompt_cache_hit_tokens,omitempty"`
	PromptCacheMissTokens   int         `json:"prompt_cache_miss_tokens,omitempty"`
	CompletionTokensDetails interface{} `json:"completion_tokens_details,omitempty"`
}

// 响应体结构
type CompletionResponse struct {
	ID                string             `json:"id"`
	Object            string             `json:"object"`
	Created           int                `json:"created"`
	Model             string             `json:"model"`
	Choices           []CompletionChoice `json:"choices"`
	Usage             CompletionUsage    `json:"usage"`
	SystemFingerprint string             `json:"system_fingerprint,omitemtpy"`
}
