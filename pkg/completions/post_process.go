package completions

import (
	"strings"
)

// 补全后置处理器类型
type PostprocessorType string

const (
	DiscardExtremeRepetition string = "discard-extreme_repetition"
	DiscardNotMatchLanguage  string = "discard-not_match_language"
	DiscardInvalidBrackets   string = "discard-invalid_brackets"
	DiscardSyntaxError       string = "discard-syntax_error"
	CutRepetitiveText        string = "cut-repetitive_text"
	CutPrefixOverlap         string = "cut-prefix_overlap"
	CutSuffixOverlap         string = "cut-suffix_overlap"
	CutCssContent            string = "cut-css_content"
	CutSyntaxError           string = "cut-syntax_error"
)

// 补全后置处理器上下文
type PostprocessorContext struct {
	Language       string `json:"language"`
	CompletionCode string `json:"completion_code"`
	Prefix         string `json:"prefix"`
	Suffix         string `json:"suffix"`
}

// 抽象补全后置处理器接口
type Postprocessor interface {
	Process(context *PostprocessorContext) bool
	GetProcessorType() string
}

//------------------------------------------------------------------------------
//	PostprocessorChain
//------------------------------------------------------------------------------

// 补全后置处理器链
type PostprocessorChain struct {
	discarders    []Postprocessor
	cutters       []Postprocessor
	requestID     string
	hitProcessors []string
}

func NewPostprocessorChain(discarders, cutters []Postprocessor, requestID string) *PostprocessorChain {
	return &PostprocessorChain{
		discarders:    discarders,
		cutters:       cutters,
		requestID:     requestID,
		hitProcessors: make([]string, 0),
	}
}

func NewDefaultPostprocessorChain(requestID string) *PostprocessorChain {
	return NewPostprocessorChain(
		[]Postprocessor{
			&ExtremeRepetitionDiscarder{},
			&NotMatchLanguageDiscarder{},
			&SyntaxErrorDiscarder{},
		},
		[]Postprocessor{
			&RepetitiveTextCutter{},
			&PrefixOverlapCutter{},
			&SuffixOverlapCutter{},
			&SyntaxErrorCutter{},
		},
		requestID,
	)
}

func (c *PostprocessorChain) processDiscard(context *PostprocessorContext) bool {
	for _, dicarder := range c.discarders {
		if dicarder.Process(context) {
			c.hitProcessors = append(c.hitProcessors, dicarder.GetProcessorType())
			return true
		}
	}
	return false
}

func (c *PostprocessorChain) processCut(context *PostprocessorContext) string {
	for _, cutter := range c.cutters {
		if cutter.Process(context) {
			c.hitProcessors = append(c.hitProcessors, cutter.GetProcessorType())
		}
	}
	return context.CompletionCode
}

func (c *PostprocessorChain) Process(context *PostprocessorContext) string {
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

func (c *PostprocessorChain) GetHitProcessors() []string {
	return c.hitProcessors
}

// ------------------------------------------------------------------------------
//
//	Postprocessors
//
// ------------------------------------------------------------------------------
// 极端重复内容丢弃处理器
type ExtremeRepetitionDiscarder struct{}

func (p *ExtremeRepetitionDiscarder) Process(context *PostprocessorContext) bool {
	// 极端重复内容丢弃
	flag, _, _ := isExtremeRepetition(context.CompletionCode)
	if !flag {
		return false
	}
	context.CompletionCode = ""
	// TODO: 添加日志记录
	// logger.Info(f"内容丢弃处理器：{p.GetProcessorType()}命中，返回空补全！最长公共子串: {lcs}, 重复行数: {repeatLines}")
	return true
}

func (p *ExtremeRepetitionDiscarder) GetProcessorType() string {
	return string(DiscardExtremeRepetition)
}

// 非匹配语言补全丢弃处理器
type NotMatchLanguageDiscarder struct{}

func (p *NotMatchLanguageDiscarder) Process(context *PostprocessorContext) bool {
	// 非python语言但是python代码，则丢弃补全内容
	if strings.ToLower(context.Language) != "python" && IsPythonText(context.CompletionCode) {
		// TODO: 添加日志记录
		// logger.Info(f"内容丢弃处理器：{p.GetProcessorType()}命中，返回空补全！")
		context.CompletionCode = ""
		return true
	}
	return false
}

func (p *NotMatchLanguageDiscarder) GetProcessorType() string {
	return string(DiscardNotMatchLanguage)
}

// RepetitiveTextCutter 重复文本裁剪处理器
type RepetitiveTextCutter struct{}

func (p *RepetitiveTextCutter) Process(context *PostprocessorContext) bool {
	// 补全内容去重
	processedCode := cutRepetitiveText(context.CompletionCode)
	if processedCode != context.CompletionCode {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。裁剪后的补全结果为：{processedCode}")
		context.CompletionCode = processedCode
		return true
	}
	return false
}

func (p *RepetitiveTextCutter) GetProcessorType() string {
	return string(CutRepetitiveText)
}

// PrefixOverlapCutter 前缀重叠裁剪处理器
type PrefixOverlapCutter struct{}

func (p *PrefixOverlapCutter) Process(context *PostprocessorContext) bool {
	// 补全内容前缀重复处理
	// 使用默认的cutLine参数值3
	processedCode := cutPrefixOverlap(context.CompletionCode, context.Prefix, context.Suffix, 3)
	if processedCode != context.CompletionCode {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。裁剪后的补全结果为：{processedCode}")
		context.CompletionCode = processedCode
		return true
	}
	return false
}

func (p *PrefixOverlapCutter) GetProcessorType() string {
	return string(CutPrefixOverlap)
}

// SuffixOverlapCutter 后缀重叠裁剪处理器
type SuffixOverlapCutter struct{}

func (p *SuffixOverlapCutter) Process(context *PostprocessorContext) bool {
	// 补全内容重叠裁剪
	// 使用默认的cutLine参数值3和ignoreOverlapLen参数值8
	processedCode := cutSuffixOverlap(context.CompletionCode, context.Prefix, context.Suffix, 3, 8)
	if processedCode != context.CompletionCode {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。裁剪后的补全结果为：{processedCode}")
		context.CompletionCode = processedCode
		return true
	}
	return false
}

func (p *SuffixOverlapCutter) GetProcessorType() string {
	return string(CutSuffixOverlap)
}

// 以下是工具函数的占位符，后续需要从common.py移植实现

// 无效括号丢弃处理器
type InvalidBracketsDiscarder struct{}

func (p *InvalidBracketsDiscarder) Process(context *PostprocessorContext) bool {
	// 检查补全代码中的括号是否完整
	if !IsValidBrackets(context.CompletionCode) {
		// TODO: 添加日志记录
		// logger.Info(f"内容丢弃处理器：{p.GetProcessorType()}命中，返回空补全！")
		return true
	}
	return false
}

func (p *InvalidBracketsDiscarder) GetProcessorType() string {
	return string(DiscardInvalidBrackets)
}

// CSS内容裁剪处理器
type CssContentCutter struct{}

func (p *CssContentCutter) ProcessCut(context *PostprocessorContext) bool {
	// 如果是非CSS语言但是包含CSS内容，则去除CSS内容
	if strings.ToLower(context.Language) != "css" && JudgeCss(context.Language, context.CompletionCode, 0.7) {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。去除CSS内容")
		context.CompletionCode = ""
		return true
	}
	return false
}

func (p *CssContentCutter) GetProcessorType() string {
	return string(CutCssContent)
}

// SyntaxErrorDiscarder 语法错误丢弃处理器
type SyntaxErrorDiscarder struct{}

func (p *SyntaxErrorDiscarder) Process(context *PostprocessorContext) bool {
	// 检查补全代码是否存在语法错误
	if !isCodeSyntax(context.Language, context.CompletionCode, context.Prefix, context.Suffix) {
		// TODO: 添加日志记录
		// logger.Info(f"内容丢弃处理器：{p.GetProcessorType()}命中，返回空补全！")
		context.CompletionCode = ""
		return true
	}
	return false
}

func (p *SyntaxErrorDiscarder) GetProcessorType() string {
	return string(DiscardSyntaxError)
}

// SyntaxErrorCutter 语法错误裁剪处理器
type SyntaxErrorCutter struct{}

func (p *SyntaxErrorCutter) Process(context *PostprocessorContext) bool {
	// 使用 TreeSitter 进行语法错误拦截和代码裁剪
	tsUtil := NewTreeSitterUtil(context.Language)
	if tsUtil == nil {
		return false
	}

	processedCode := tsUtil.InterceptSyntaxErrorCode(context.CompletionCode, context.Prefix, context.Suffix)
	if processedCode != context.CompletionCode {
		// TODO: 添加日志记录
		// logger.Info(f"内容裁剪处理器：{p.GetProcessorType()}命中。裁剪后的补全结果为：{processedCode}")
		context.CompletionCode = processedCode
		return true
	}
	return false
}

func (p *SyntaxErrorCutter) GetProcessorType() string {
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
