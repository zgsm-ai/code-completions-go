package completions

import (
	"strings"
)

// ContentDiscardType 内容丢弃类型
type ContentDiscardType string

const (
	DiscardExtremeRepetition          ContentDiscardType = "discard-extreme_repetition"
	DiscardNotMatchCompletionLanguage ContentDiscardType = "discard-not_match_completion_language"
	DiscardInvalidBrackets            ContentDiscardType = "discard-invalid_brackets"
	DiscardSyntaxError                ContentDiscardType = "discard-syntax_error"
)

// ContentCutType 内容裁剪类型
type ContentCutType string

const (
	CutRepetitiveText ContentCutType = "cut-repetitive_text"
	CutPrefixOverlap  ContentCutType = "cut-prefix_overlap"
	CutSuffixOverlap  ContentCutType = "cut-suffix_overlap"
	CutCssContent     ContentCutType = "cut-css_content"
	CutSyntaxError    ContentCutType = "cut-syntax_error"
)

// CompletionPostprocessorContext 补全后置处理器上下文
type CompletionPostprocessorContext struct {
	Language       string `json:"language"`
	CompletionCode string `json:"completion_code"`
	Prefix         string `json:"prefix"`
	Suffix         string `json:"suffix"`
}

// AbstractCompletionPostprocessor 抽象补全后置处理器接口
type AbstractCompletionPostprocessor interface {
	ProcessDiscard(context *CompletionPostprocessorContext) bool
	ProcessCut(context *CompletionPostprocessorContext) string
	GetProcessorType() string
}

// DiscardExtremeRepetitionProcessor 极端重复内容丢弃处理器
type DiscardExtremeRepetitionProcessor struct{}

func (p *DiscardExtremeRepetitionProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	// 极端重复内容丢弃
	flag, _, _ := isExtremeRepetition(context.CompletionCode)
	if !flag {
		return false
	}

	// TODO: 添加日志记录
	// logger.Info(f"内容丢弃处理器：{p.GetProcessorType()}命中，返回空补全！最长公共子串: {lcs}, 重复行数: {repeatLines}")
	return true
}

func (p *DiscardExtremeRepetitionProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	return context.CompletionCode
}

func (p *DiscardExtremeRepetitionProcessor) GetProcessorType() string {
	return string(DiscardExtremeRepetition)
}

// DiscardNotMatchCompletionLanguageProcessor 非匹配语言补全丢弃处理器
type DiscardNotMatchCompletionLanguageProcessor struct{}

func (p *DiscardNotMatchCompletionLanguageProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	// 非python语言但是python代码，则丢弃补全内容
	if strings.ToLower(context.Language) != "python" && isPythonText(context.CompletionCode) {
		// TODO: 添加日志记录
		// logger.Info(f"内容丢弃处理器：{p.GetProcessorType()}命中，返回空补全！")
		return true
	}
	return false
}

func (p *DiscardNotMatchCompletionLanguageProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	return context.CompletionCode
}

func (p *DiscardNotMatchCompletionLanguageProcessor) GetProcessorType() string {
	return string(DiscardNotMatchCompletionLanguage)
}

// CutRepetitiveTextProcessor 重复文本裁剪处理器
type CutRepetitiveTextProcessor struct{}

func (p *CutRepetitiveTextProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	return false
}

func (p *CutRepetitiveTextProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	// 补全内容去重
	processedCode := cutRepetitiveText(context.CompletionCode)
	if processedCode != context.CompletionCode {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。裁剪后的补全结果为：{processedCode}")
	}
	return processedCode
}

func (p *CutRepetitiveTextProcessor) GetProcessorType() string {
	return string(CutRepetitiveText)
}

// CutPrefixOverlapProcessor 前缀重叠裁剪处理器
type CutPrefixOverlapProcessor struct{}

func (p *CutPrefixOverlapProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	return false
}

func (p *CutPrefixOverlapProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	// 补全内容前缀重复处理
	// 使用默认的cutLine参数值3
	processedCode := cutPrefixOverlap(context.CompletionCode, context.Prefix, context.Suffix, 3)
	if processedCode != context.CompletionCode {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。裁剪后的补全结果为：{processedCode}")
	}
	return processedCode
}

func (p *CutPrefixOverlapProcessor) GetProcessorType() string {
	return string(CutPrefixOverlap)
}

// CutSuffixOverlapProcessor 后缀重叠裁剪处理器
type CutSuffixOverlapProcessor struct{}

func (p *CutSuffixOverlapProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	return false
}

func (p *CutSuffixOverlapProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	// 补全内容重叠裁剪
	// 使用默认的cutLine参数值3和ignoreOverlapLen参数值8
	processedCode := cutSuffixOverlap(context.CompletionCode, context.Prefix, context.Suffix, 3, 8)
	if processedCode != context.CompletionCode {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。裁剪后的补全结果为：{processedCode}")
	}
	return processedCode
}

func (p *CutSuffixOverlapProcessor) GetProcessorType() string {
	return string(CutSuffixOverlap)
}

// CompletionPostprocessorChain 补全后置处理器链
type CompletionPostprocessorChain struct {
	postprocessors []AbstractCompletionPostprocessor
	requestID      string
	hitProcessors  []string
}

func NewCompletionPostprocessorChain(postprocessors []AbstractCompletionPostprocessor, requestID string) *CompletionPostprocessorChain {
	return &CompletionPostprocessorChain{
		postprocessors: postprocessors,
		requestID:      requestID,
		hitProcessors:  make([]string, 0),
	}
}

func (c *CompletionPostprocessorChain) processDiscard(context *CompletionPostprocessorContext) bool {
	for _, postprocessor := range c.postprocessors {
		if postprocessor.ProcessDiscard(context) {
			c.hitProcessors = append(c.hitProcessors, postprocessor.GetProcessorType())
			return true
		}
	}
	return false
}

func (c *CompletionPostprocessorChain) processCut(context *CompletionPostprocessorContext) string {
	for _, postprocessor := range c.postprocessors {
		cutText := postprocessor.ProcessCut(context)
		if cutText != context.CompletionCode {
			c.hitProcessors = append(c.hitProcessors, postprocessor.GetProcessorType())
			context.CompletionCode = cutText
		}
	}
	return context.CompletionCode
}

func (c *CompletionPostprocessorChain) Process(context *CompletionPostprocessorContext) string {
	// 先处理内容丢弃情况，再处理内容裁剪情况
	// TODO: 添加日志记录
	// logger.Info(f"后处理前的补全内容为：{context.CompletionCode}", requestID=c.requestID)

	discardFlag := c.processDiscard(context)
	if discardFlag {
		context.CompletionCode = ""
		return context.CompletionCode
	}

	c.processCut(context)

	// 后置验证：去除补全内容末尾的空格
	if context.CompletionCode != "" {
		context.CompletionCode = strings.TrimRight(context.CompletionCode, " \t\n\r")
	}

	return context.CompletionCode
}

func (c *CompletionPostprocessorChain) GetHitProcessors() []string {
	return c.hitProcessors
}

// CompletionPostprocessorFactory 补全后置处理器工厂
type CompletionPostprocessorFactory struct{}

func (f *CompletionPostprocessorFactory) CreateDefault(requestID string) *CompletionPostprocessorChain {
	return NewCompletionPostprocessorChain(
		[]AbstractCompletionPostprocessor{
			&DiscardExtremeRepetitionProcessor{},
			&DiscardNotMatchCompletionLanguageProcessor{},
			&DiscardSyntaxErrorProcessor{},
			&CutRepetitiveTextProcessor{},
			&CutPrefixOverlapProcessor{},
			&CutSuffixOverlapProcessor{},
			&CutSyntaxErrorProcessor{},
		},
		requestID,
	)
}

func (f *CompletionPostprocessorFactory) Create(postprocessors []AbstractCompletionPostprocessor) *CompletionPostprocessorChain {
	return NewCompletionPostprocessorChain(postprocessors, "")
}

// 以下是工具函数的占位符，后续需要从common.py移植实现

// isPythonText 判断是否为Python文本
func isPythonText(text string) bool {
	return IsPythonText(text)
}

// isValidContent 判断内容是否有效
func isValidContent(text string) bool {
	return strings.TrimSpace(text) != ""
}

// DiscardInvalidBracketsProcessor 无效括号丢弃处理器
type DiscardInvalidBracketsProcessor struct{}

func (p *DiscardInvalidBracketsProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	// 检查补全代码中的括号是否完整
	if !IsValidBrackets(context.CompletionCode) {
		// TODO: 添加日志记录
		// logger.Info(f"内容丢弃处理器：{p.GetProcessorType()}命中，返回空补全！")
		return true
	}
	return false
}

func (p *DiscardInvalidBracketsProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	return context.CompletionCode
}

func (p *DiscardInvalidBracketsProcessor) GetProcessorType() string {
	return string(DiscardInvalidBrackets)
}

// CutCssContentProcessor CSS内容裁剪处理器
type CutCssContentProcessor struct{}

func (p *CutCssContentProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	return false
}

func (p *CutCssContentProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	// 如果是非CSS语言但是包含CSS内容，则去除CSS内容
	if strings.ToLower(context.Language) != "css" && JudgeCss(context.Language, context.CompletionCode, 0.7) {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。去除CSS内容")
		return ""
	}
	return context.CompletionCode
}

func (p *CutCssContentProcessor) GetProcessorType() string {
	return string(CutCssContent)
}

// DiscardSyntaxErrorProcessor 语法错误丢弃处理器
type DiscardSyntaxErrorProcessor struct{}

func (p *DiscardSyntaxErrorProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	// 检查补全代码是否存在语法错误
	if !isCodeSyntax(context.Language, context.CompletionCode, context.Prefix, context.Suffix) {
		// TODO: 添加日志记录
		// logger.Info(f"内容丢弃处理器：{p.GetProcessorType()}命中，返回空补全！")
		return true
	}
	return false
}

func (p *DiscardSyntaxErrorProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	return context.CompletionCode
}

func (p *DiscardSyntaxErrorProcessor) GetProcessorType() string {
	return string(DiscardSyntaxError)
}

// CutSyntaxErrorProcessor 语法错误裁剪处理器
type CutSyntaxErrorProcessor struct{}

func (p *CutSyntaxErrorProcessor) ProcessDiscard(context *CompletionPostprocessorContext) bool {
	return false
}

func (p *CutSyntaxErrorProcessor) ProcessCut(context *CompletionPostprocessorContext) string {
	// 使用 TreeSitter 进行语法错误拦截和代码裁剪
	tsUtil := NewTreeSitterUtil(context.Language)
	if tsUtil == nil {
		return context.CompletionCode
	}
	
	processedCode := tsUtil.InterceptSyntaxErrorCode(context.CompletionCode, context.Prefix, context.Suffix, 0, 3500)
	if processedCode != context.CompletionCode {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。裁剪后的补全结果为：{processedCode}")
	}
	return processedCode
}

func (p *CutSyntaxErrorProcessor) GetProcessorType() string {
	return string(CutSyntaxError)
}

// isCodeSyntax 检查代码语法是否正确
func isCodeSyntax(language, code, prefix, suffix string) bool {
	tsUtil := NewTreeSitterUtil(language)
	if tsUtil == nil {
		return true
	}
	
	// 提取准确的代码块前后缀
	newPrefix, newSuffix := tsUtil.ExtractAccurateBlockPrefixSuffix(prefix, suffix)
	
	// 检查语法
	return tsUtil.IsCodeSyntax(newPrefix + code + newSuffix)
}
