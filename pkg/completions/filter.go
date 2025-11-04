package completions

import (
	"fmt"
	"math"
	"strings"
	"time"

	"code-completion/pkg/config"
	"code-completion/pkg/logger"

	"go.uber.org/zap"
)

// 拒绝原因枚举
type RejectCode string

const (
	Accepted          RejectCode = "ACCEPTED"
	LowHiddenScore    RejectCode = "LOW_HIDDEN_SCORE"
	AuthFail          RejectCode = "AUTH_FAIL"
	FeatureNotSupport RejectCode = "FEATURE_NOT_SUPPORT"
)

// 补全过滤器接口
type Filter interface {
	Judge(data *CompletionRequest) RejectCode
}

// 语言特性过滤器
type LanguageFeatureFilter struct {
	codeFilters *CodeFilters
}

// 创建语言特性过滤器
func NewLanguageFeatureFilter(cfg *config.CompletionWrapperConfig) *LanguageFeatureFilter {
	thresholdScore := cfg.ThresholdScore
	if thresholdScore == 0 {
		thresholdScore = 0.3
	}
	strPattern := cfg.StrPattern
	if strPattern == "" {
		strPattern = `import +.*|from +.*|from +.* import *.*`
	}

	treePattern := cfg.TreePattern
	if treePattern == "" {
		treePattern = `\(comment.*|\(string.*|\(set \(string.*|\(dictionary.*|\(integer.*|\(list.*|\(tuple.*`
	}
	lineCountThreshold := cfg.LineCountThreshold
	if lineCountThreshold == 0 {
		lineCountThreshold = 5
	}

	endTag := cfg.EndTag
	if endTag == "" {
		endTag = "('>',';','}',')')"
	}

	return &LanguageFeatureFilter{
		codeFilters: NewCodeFilters(
			thresholdScore,
			lineCountThreshold,
			strPattern,
			treePattern,
			endTag,
		),
	}
}

func (h *LanguageFeatureFilter) Judge(data *CompletionRequest) RejectCode {
	// 跳过手动触发模式
	if strings.ToUpper(data.TriggerMode) == "MANUAL" {
		return Accepted
	}

	// 检查是否需要代码补全
	if h.codeFilters.NeedCode(data) {
		return Accepted
	}

	return FeatureNotSupport
}

// 低隐藏分数过滤器
type HiddenScoreFilter struct {
	hideScoreConfig *HideScoreConfig
}

// 创建隐藏分数过滤器
func NewHiddenScoreFilter(cfg *config.CompletionWrapperConfig) *HiddenScoreFilter {
	thresholdScore := cfg.ThresholdScore
	if thresholdScore == 0 {
		thresholdScore = 0.3
	}

	return &HiddenScoreFilter{
		hideScoreConfig: NewHideScoreConfig("./config/hide_score.yml", thresholdScore),
	}
}

func (h *HiddenScoreFilter) Judge(data *CompletionRequest) RejectCode {
	// 跳过手动触发和继续补全模式
	mode := strings.ToUpper(data.TriggerMode)
	if mode == "MANUAL" || mode == "CONTINUE" {
		return Accepted
	}

	// 计算隐藏分数
	if data.CalculateHideScore == nil {
		return Accepted
	}

	score := 0.0
	if data.CalculateHideScore.DocumentLength != 0 {
		if data.PromptOptions != nil {
			data.CalculateHideScore.Prefix = data.PromptOptions.Prefix
		}
		score = h.hideScoreConfig.CalculateHideScore(data.CalculateHideScore, data.LanguageID)
	}

	// 将分数更新到请求数据中（问题4修复）
	if data.Extra == nil {
		data.Extra = make(map[string]interface{})
	}
	data.Extra["score"] = score

	// 通过配置阈值来过滤隐藏分低的补全
	if score < h.hideScoreConfig.ThresholdScore {
		// 添加日志记录（问题1修复）
		logger.Debug("低隐藏分数拒绝补全",
			zap.Float64("score", score),
			zap.Float64("threshold", h.hideScoreConfig.ThresholdScore),
			zap.String("completion_id", data.CompletionID),
			zap.String("language", data.LanguageID))
		return LowHiddenScore
	}

	return Accepted
}

// 补全拒绝规则链
type FilterChain struct {
	filters []Filter
}

// 创建新的拒绝规则链
func NewFilterChain(cfg *config.CompletionWrapperConfig) *FilterChain {
	handlers := make([]Filter, 0)

	if !cfg.DisableScore {
		handlers = append(handlers, NewHiddenScoreFilter(cfg))
	}

	if !cfg.DisableLanguageFeature {
		handlers = append(handlers, NewLanguageFeatureFilter(cfg))
	}

	return &FilterChain{
		filters: handlers,
	}
}

// 处理补全请求，只要命中一个规则就拒绝补全
func (c *FilterChain) Handle(data *CompletionRequest) error {
	for _, handler := range c.filters {
		if rejectCode := handler.Judge(data); rejectCode != Accepted {
			return fmt.Errorf("%s", rejectCode)
		}
	}
	return nil
}

// CodeFilters 代码过滤器
type CodeFilters struct {
	ThresholdScore     float64
	StrPattern         string
	TreePattern        string
	FIMIndicator       string
	EndTag             string
	LineCountThreshold int
}

// 创建代码过滤器
func NewCodeFilters(thresholdScore float64, lineCountThreshold int, strPattern, treePattern, endTag string) *CodeFilters {
	return &CodeFilters{
		ThresholdScore:     thresholdScore,
		StrPattern:         strPattern,
		TreePattern:        treePattern,
		FIMIndicator:       "<FILL_HERE>",
		EndTag:             endTag,
		LineCountThreshold: lineCountThreshold,
	}
}

func (c *CodeFilters) NeedCode(data *CompletionRequest) bool {
	// 是否需要触发模型进行自动补全编码

	// 暂时关闭，参考相关文档
	// if c.tooFewLines(data) {
	//     return false
	// }

	if c.cursorIsAtTheEnd(data) {
		return false
	}

	if c.textAfterFillHereStartWithWord(data) {
		return false
	}

	// 简化实现，其他复杂的过滤逻辑暂时关闭
	// 可以根据需要逐步启用其他过滤条件

	return true
}

func (c *CodeFilters) splitPrompt(prompt string) (string, string) {
	textBeforeCursor, textAfterCursor := "", ""
	if strings.Contains(prompt, c.FIMIndicator) {
		parts := strings.Split(prompt, c.FIMIndicator)
		if len(parts) >= 2 {
			textBeforeCursor = parts[len(parts)-2]
			textAfterCursor = parts[len(parts)-1]
		} else {
			textBeforeCursor = prompt
			textAfterCursor = ""
		}
	}
	return textBeforeCursor, textAfterCursor
}

func (c *CodeFilters) cursorIsAtTheEnd(data *CompletionRequest) bool {
	// 光标位于行尾的直接不触发补全
	// 行尾定义：光标左侧是'>'、';'、'}'、')'，右侧是换行符号

	textBeforeCursor, textAfterCursor := c.splitPrompt(data.Prompt)
	if textBeforeCursor != "" && textAfterCursor != "" {
		// 解析endTag
		endTags := c.parseEndTag()
		for _, tag := range endTags {
			if strings.HasSuffix(strings.ReplaceAll(textBeforeCursor, " ", ""), tag) {
				// 检查右侧是否是空行
				lines := strings.Split(textAfterCursor, "\n")
				if len(lines) > 0 && strings.TrimSpace(lines[0]) == "" {
					// fmt.Printf("光标位于行尾，跳过自动补全\n")
					return true
				}
			}
		}
	}
	return false
}

func (c *CodeFilters) parseEndTag() []string {
	// 解析endTag配置，格式如 "('>',';','}',')')"
	endTag := strings.TrimSpace(c.EndTag)
	endTag = strings.TrimPrefix(endTag, "(")
	endTag = strings.TrimSuffix(endTag, ")")
	endTag = strings.TrimPrefix(endTag, "'")
	endTag = strings.TrimSuffix(endTag, "'")

	tags := strings.Split(endTag, "','")
	result := make([]string, 0)
	for _, tag := range tags {
		tag = strings.TrimSpace(tag)
		if tag != "" {
			result = append(result, tag)
		}
	}
	return result
}

func (c *CodeFilters) textAfterFillHereStartWithWord(data *CompletionRequest) bool {
	// 补全后面直接是英文字母开头或数字的不补全，比如修改变量名称的场景
	_, textAfterCursor := c.splitPrompt(data.Prompt)
	if textAfterCursor != "" {
		firstChar := textAfterCursor[0]
		if (firstChar >= 'a' && firstChar <= 'z') || (firstChar >= 'A' && firstChar <= 'Z') || (firstChar >= '0' && firstChar <= '9') {
			// fmt.Printf("光标后面是字符`%c`，跳过自动补全\n", firstChar)
			return true
		}
	}
	return false
}

func (c *CodeFilters) tooFewLines(data *CompletionRequest) bool {
	// prompt行数太少不触发补全，排除空行场景
	lines := strings.Split(data.Prompt, "\n")
	nonEmptyLines := make([]string, 0)
	for _, line := range lines {
		if strings.TrimSpace(line) != "" {
			nonEmptyLines = append(nonEmptyLines, line)
		}
	}
	lineCount := len(nonEmptyLines)
	if lineCount < c.LineCountThreshold {
		// fmt.Printf("prompt行数%d小于阈值%d，跳过自动补全\n", lineCount, c.LineCountThreshold)
		return true
	}
	return false
}

// HideScoreConfig 隐藏分数配置
type HideScoreConfig struct {
	ThresholdScore                  float64
	ContextualFilterLanguageMap     map[string]int
	ContextualFilterWeights         []float64
	ContextualFilterAcceptThreshold float64
	ContextualFilterIntercept       float64
	ContextualFilterCharacterMap    map[string]int
}

// NewHideScoreConfig 创建隐藏分数配置
func NewHideScoreConfig(configPath string, thresholdScore float64) *HideScoreConfig {
	// 默认配置，模拟YAML文件中的配置
	config := &HideScoreConfig{
		ContextualFilterLanguageMap: map[string]int{
			"python": 0, "javascript": 1, "typescript": 2, "java": 3, "go": 4,
			"c": 5, "cpp": 6, "csharp": 7, "php": 8, "ruby": 9,
			"rust": 10, "kotlin": 11, "scala": 12, "swift": 13, "objective-c": 14,
		},
		ContextualFilterWeights: []float64{
			0.99,   // 上一个标签的权重
			0.7,    // 当前行光标后为空的权重
			-0.17,  // 时间间隔的权重
			-0.22,  // 前缀尾行长度的权重
			0.13,   // 后缀长度的权重
			-0.007, // 文档长度的权重
			0.005,  // 光标所在文档位置的权重
			0.41,   // 光标位置与文档长度比值的权重
			// 语言权重，从第8个位置开始，每种语言一个权重
			-0.1, -0.08, -0.06, -0.04, -0.02, 0.0, 0.02, 0.04, 0.06, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18,
			// 字符权重，从第29个位置开始
		},
		ContextualFilterAcceptThreshold: 0.5,
		ContextualFilterIntercept:       -0.3,
		ContextualFilterCharacterMap: map[string]int{
			" ": 0, "\t": 1, "\n": 2, "(": 3, ")": 4, "[": 5, "]": 6, "{": 7, "}": 8,
			",": 9, ";": 10, ":": 11, ".": 12, "=": 13, "+": 14, "-": 15, "*": 16, "/": 17,
			"\\": 18, "\"": 19, "'": 20, "<": 21, ">": 22, "?": 23, "!": 24, "@": 25, "#": 26,
			"$": 27, "%": 28, "^": 29, "&": 30, "|": 31, "~": 32, "`": 33,
		},
	}

	if thresholdScore > 0 {
		config.ThresholdScore = thresholdScore
	} else {
		config.ThresholdScore = 0.3
	}

	return config
}

func (h *HideScoreConfig) CalculateHideScore(calculateHideScore *CalculateHideScore, language string) float64 {
	// 判断光标权重
	whitespaceAfterCursor := 0.0
	if calculateHideScore.IsWhitespaceAfterCursor {
		whitespaceAfterCursor = 1.0
	}

	// 触发时间间隔
	timeSincePreviousLabel := float64(time.Now().Unix()*1000-calculateHideScore.PreviousLabelTimestamp) / 1000.0

	// 3.6最小值参考copilot的设置
	timeSincePreviousLabelLog := math.Log(1.0 + math.Max(3.6, timeSincePreviousLabel))

	prefixLengthLog := 0.0
	prefixLastCharWeight := 0
	prefixStr := calculateHideScore.Prefix

	if prefixStr != "" {
		prefixLengthLog = math.Log(1.0 + float64(h.getLastLineLength(prefixStr)))
		prefixLastChar := prefixStr[len(prefixStr)-1:]
		if weight, exists := h.ContextualFilterCharacterMap[prefixLastChar]; exists {
			prefixLastCharWeight = weight
		}
	}

	suffixLengthLog := 0.0
	suffixLastCharWeight := 0

	// 参考const g = h.trimEnd(); 应该把换行符号也删掉
	trimmedSuffixStr := strings.TrimRight(prefixStr, " \t\n\r")
	if trimmedSuffixStr != "" {
		suffixLengthLog = math.Log(1.0 + float64(h.getLastLineLength(trimmedSuffixStr)))
		suffixLastChar := trimmedSuffixStr[len(trimmedSuffixStr)-1:]
		if weight, exists := h.ContextualFilterCharacterMap[suffixLastChar]; exists {
			suffixLastCharWeight = weight
		}
	}

	documentLengthLog := math.Log(1.0 + math.Max(float64(calculateHideScore.DocumentLength), 0.0))
	promptEndPosLog := math.Log(1.0 + math.Max(float64(calculateHideScore.PromptEndPos), 0.0))
	promptEndPosRatio := (float64(calculateHideScore.PromptEndPos) + 0.5) / (1.0 + float64(calculateHideScore.DocumentLength))

	// 若不支持该语言，默认走python
	languageWeight := 4 // python的默认值
	if weight, exists := h.ContextualFilterLanguageMap[language]; exists {
		languageWeight = weight
	}

	// 初始值-0.3
	score := h.ContextualFilterIntercept

	// 上一个标签的权重(上一次接受的话，下一次基本都会给予补全) +0.99
	if len(h.ContextualFilterWeights) > 0 {
		score += h.ContextualFilterWeights[0] * float64(calculateHideScore.PreviousLabel)
	}

	// 当前行光标后为空的话倾向补全 + 0.7
	if len(h.ContextualFilterWeights) > 1 {
		score += h.ContextualFilterWeights[1] * whitespaceAfterCursor
	}

	// 时间间隔的权重，上一次触发的时间越久越不补全 - 0.17
	if len(h.ContextualFilterWeights) > 2 {
		score += h.ContextualFilterWeights[2] * timeSincePreviousLabelLog
	}

	// 前缀尾行长度的权重，尾行越长越不补全 - 0.22
	if len(h.ContextualFilterWeights) > 3 {
		score += h.ContextualFilterWeights[3] * prefixLengthLog
	}

	// 前缀去除空行或者空格后尾行长度的权重（去除空格或空行后的尾行），后缀越长越补全 + 0.13
	if len(h.ContextualFilterWeights) > 4 {
		score += h.ContextualFilterWeights[4] * suffixLengthLog
	}

	// 文档长度的权重，越长越不补 - 0.007
	if len(h.ContextualFilterWeights) > 5 {
		score += h.ContextualFilterWeights[5] * documentLengthLog
	}

	// 光标所在文档位置的权重，越靠后越补 + 0.005
	if len(h.ContextualFilterWeights) > 6 {
		score += h.ContextualFilterWeights[6] * promptEndPosLog
	}

	// 光标位置与文档长度的比值的权重，越靠后越补 + 0.41
	if len(h.ContextualFilterWeights) > 7 {
		score += h.ContextualFilterWeights[7] * promptEndPosRatio
	}

	// 语言权重
	languageWeightIndex := 8 + languageWeight
	if len(h.ContextualFilterWeights) > int(languageWeightIndex) {
		score += h.ContextualFilterWeights[languageWeightIndex]
	}

	// 前缀的最后一个字符的权重
	prefixCharWeightIndex := 29 + prefixLastCharWeight
	if len(h.ContextualFilterWeights) > int(prefixCharWeightIndex) {
		score += h.ContextualFilterWeights[prefixCharWeightIndex]
	}

	// 前缀最后一个有效行的最后一个字符的权重
	suffixCharWeightIndex := 125 + suffixLastCharWeight
	if len(h.ContextualFilterWeights) > int(suffixCharWeightIndex) {
		score += h.ContextualFilterWeights[suffixCharWeightIndex]
	}

	probabilityAccept := 1.0 / (1.0 + math.Exp(-score))
	return probabilityAccept
}

func (h *HideScoreConfig) getLastLineLength(text string) int {
	if text == "" {
		return 0
	}
	lines := strings.Split(text, "\n")
	if len(lines) == 0 {
		return 0
	}
	return len(lines[len(lines)-1])
}
