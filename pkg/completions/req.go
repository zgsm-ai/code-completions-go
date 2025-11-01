package completions

// 补全请求结构
type CompletionRequest struct {
	Model              string                 `json:"model,omitempty"`
	Prompt             string                 `json:"prompt"`
	PromptOptions      *PromptOptions         `json:"prompt_options,omitempty"`
	LanguageID         string                 `json:"language_id,omitempty"`
	ClientID           string                 `json:"client_id,omitempty"`
	CompletionID       string                 `json:"completion_id,omitempty"`
	ProjectPath        string                 `json:"project_path,omitempty"`
	FileProjectPath    string                 `json:"file_project_path,omitempty"`
	ImportContent      string                 `json:"import_content,omitempty"`
	Temperature        float64                `json:"temperature,omitempty"`
	TriggerMode        string                 `json:"trigger_mode,omitempty"`
	ParentID           string                 `json:"parent_id,omitempty"`
	Stop               []string               `json:"stop,omitempty"`
	BetaMode           bool                   `json:"beta_mode,omitempty"`
	Verbose            bool                   `json:"verbose,omitempty"`
	Extra              map[string]interface{} `json:"extra,omitempty"`
	CalculateHideScore *CalculateHideScore    `json:"calculate_hide_score,omitempty"`
}

// 提示词选项
type PromptOptions struct {
	Prefix           string `json:"prefix,omitempty"`
	Suffix           string `json:"suffix,omitempty"`
	CursorLinePrefix string `json:"cursor_line_prefix,omitempty"`
	CursorLineSuffix string `json:"cursor_line_suffix,omitempty"`
	CodeContext      string `json:"code_context,omitempty"`
}

// 计算隐藏分数配置
type CalculateHideScore struct {
	IsWhitespaceAfterCursor bool   `json:"is_whitespace_after_cursor"`
	Prefix                  string `json:"prefix,omitempty"`
	DocumentLength          int    `json:"document_length"`
	PromptEndPos            int    `json:"prompt_end_pos"`
	PreviousLabel           int    `json:"previous_label"`
	PreviousLabelTimestamp  int64  `json:"previous_label_timestamp"`
}
