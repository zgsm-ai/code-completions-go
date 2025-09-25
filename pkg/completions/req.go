package completions

// CompletionRequest 补全请求结构
type CompletionRequest struct {
	Prompt             string                 `json:"prompt"`
	PromptOptions      *PromptOptions         `json:"prompt_options,omitempty"`
	LanguageID         string                 `json:"language_id,omitempty"`
	ClientID           string                 `json:"client_id,omitempty"`
	ProjectPath        string                 `json:"project_path,omitempty"`
	FileProjectPath    string                 `json:"file_project_path,omitempty"`
	ImportContent      string                 `json:"import_content,omitempty"`
	Temperature        float64                `json:"temperature,omitempty"`
	ParentID           string                 `json:"parent_id,omitempty"`
	TriggerMode        string                 `json:"trigger_mode,omitempty"`
	Extra              map[string]interface{} `json:"extra,omitempty"`
	Model              string                 `json:"model,omitempty"`
	Stop               []string               `json:"stop,omitempty"`
	CompletionID       string                 `json:"completion_id,omitempty"`
	BetaMode           bool                   `json:"beta_mode,omitempty"`
	CalculateHideScore *CalculateHideScore    `json:"calculate_hide_score,omitempty"`
	CodePath           string                 `json:"code_path,omitempty"`
	UserID             string                 `json:"user_id,omitempty"`
	Repo               string                 `json:"repo,omitempty"`
	GitPath            string                 `json:"git_path,omitempty"`
}

// PromptOptions 提示词选项
type PromptOptions struct {
	Prefix           string `json:"prefix,omitempty"`
	Suffix           string `json:"suffix,omitempty"`
	CursorLinePrefix string `json:"cursor_line_prefix,omitempty"`
	CursorLineSuffix string `json:"cursor_line_suffix,omitempty"`
	CodeContext      string `json:"code_context,omitempty"`
}
