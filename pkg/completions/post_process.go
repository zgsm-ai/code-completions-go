package completions

import (
	"fmt"
	"strings"
)

// 补全后置处理器类型
type PostprocessorType string

const (
	TypeDiscarder PostprocessorType = "discarder"
	TypeCutter    PostprocessorType = "cutter"
)

const (
	DiscardExtremeRepetition string = "discard-extreme_repetition"
	DiscardNotMatchLanguage  string = "discard-not_match_language"
	DiscardInvalidBrackets   string = "discard-invalid_brackets"
	DiscardSyntaxError       string = "discard-syntax_error"
	DicardCssContent         string = "discard-css_content"
	CutRepetitiveText        string = "cut-repetitive_text"
	CutPrefixOverlap         string = "cut-prefix_overlap"
	CutSuffixOverlap         string = "cut-suffix_overlap"
	CutSyntaxError           string = "cut-syntax_error"
)

var postprocessorDefs map[string]Postprocessor = map[string]Postprocessor{
	DiscardExtremeRepetition: &ExtremeRepetitionDiscarder{},
	DiscardNotMatchLanguage:  &NotMatchLanguageDiscarder{},
	DiscardSyntaxError:       &SyntaxErrorDiscarder{},
	DiscardInvalidBrackets:   &InvalidBracketsDiscarder{},
	DicardCssContent:         &CssContentDiscarder{},
	CutRepetitiveText:        &RepetitiveTextCutter{},
	CutPrefixOverlap:         &PrefixOverlapCutter{},
	CutSuffixOverlap:         &SuffixOverlapCutter{},
	CutSyntaxError:           &SyntaxErrorCutter{},
}

// 补全后置处理器上下文
type PostprocessorContext struct {
	CompletionID   string `json:"completion_id"`
	Language       string `json:"language"`
	CompletionCode string `json:"completion_code"`
	Prefix         string `json:"prefix"`
	Suffix         string `json:"suffix"`
}

// 抽象补全后置处理器接口
type Postprocessor interface {
	Process(ctx *PostprocessorContext) bool
	Name() string
	Type() PostprocessorType
}

//------------------------------------------------------------------------------
//	PostprocessorChain
//------------------------------------------------------------------------------

// 补全后置处理器链
type PostprocessorChain struct {
	discarders    []Postprocessor
	cutters       []Postprocessor
	hitProcessors []string
}

func NewPostprocessorChain(discarders, cutters []Postprocessor) *PostprocessorChain {
	return &PostprocessorChain{
		discarders:    discarders,
		cutters:       cutters,
		hitProcessors: make([]string, 0),
	}
}

func NewPostprocessorChainByNames(names []string) (*PostprocessorChain, error) {
	dicarders := make([]Postprocessor, 0)
	cutters := make([]Postprocessor, 0)
	for _, name := range names {
		p, exists := postprocessorDefs[name]
		if !exists {
			return nil, fmt.Errorf("Invalid Postprocessor: %s", name)
		}
		if p.Type() == TypeDiscarder {
			dicarders = append(dicarders, p)
		} else {
			cutters = append(cutters, p)
		}
	}
	return NewPostprocessorChain(dicarders, cutters), nil
}

func NewDefaultPostprocessorChain() *PostprocessorChain {
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
	)
}

func (c *PostprocessorChain) processDiscard(ctx *PostprocessorContext) bool {
	for _, dicarder := range c.discarders {
		if dicarder.Process(ctx) {
			c.hitProcessors = append(c.hitProcessors, dicarder.Name())
			return true
		}
	}
	return false
}

func (c *PostprocessorChain) processCut(ctx *PostprocessorContext) bool {
	result := false
	for _, cutter := range c.cutters {
		if cutter.Process(ctx) {
			c.hitProcessors = append(c.hitProcessors, cutter.Name())
			result = true
		}
	}
	return result
}

func (c *PostprocessorChain) Process(ctx *PostprocessorContext) bool {
	// 先处理内容丢弃情况，再处理内容裁剪情况
	if c.processDiscard(ctx) {
		ctx.CompletionCode = ""
		return true
	}

	result := c.processCut(ctx)

	// 后置验证：去除补全内容末尾的空格
	if ctx.CompletionCode != "" {
		ctx.CompletionCode = strings.TrimRight(ctx.CompletionCode, " \t\n\r")
	}

	return result
}

func (c *PostprocessorChain) GetHitProcessors() []string {
	return c.hitProcessors
}

// ------------------------------------------------------------------------------
//
//	Postprocessors
//
// ------------------------------------------------------------------------------

type Discarder struct{}

func (p *Discarder) Type() PostprocessorType {
	return TypeDiscarder
}

type Cutter struct{}

func (p *Cutter) Type() PostprocessorType {
	return TypeCutter
}

// 极端重复内容丢弃处理器
type ExtremeRepetitionDiscarder struct{ Discarder }

func (p *ExtremeRepetitionDiscarder) Process(ctx *PostprocessorContext) bool {
	// 极端重复内容丢弃
	flag, _, _ := isExtremeRepetition(ctx.CompletionCode)
	if !flag {
		return false
	}
	ctx.CompletionCode = ""
	return true
}

func (p *ExtremeRepetitionDiscarder) Name() string {
	return string(DiscardExtremeRepetition)
}

// 非匹配语言补全丢弃处理器
type NotMatchLanguageDiscarder struct{ Discarder }

func (p *NotMatchLanguageDiscarder) Process(ctx *PostprocessorContext) bool {
	// 非python语言但是python代码，则丢弃补全内容
	if strings.ToLower(ctx.Language) != "python" && IsPythonText(ctx.CompletionCode) {
		ctx.CompletionCode = ""
		return true
	}
	return false
}

func (p *NotMatchLanguageDiscarder) Name() string {
	return string(DiscardNotMatchLanguage)
}

// RepetitiveTextCutter 重复文本裁剪处理器
type RepetitiveTextCutter struct{ Cutter }

func (p *RepetitiveTextCutter) Process(ctx *PostprocessorContext) bool {
	processedCode := cutRepetitiveText(ctx.CompletionCode)
	if processedCode != ctx.CompletionCode {
		ctx.CompletionCode = processedCode
		return true
	}
	return false
}

func (p *RepetitiveTextCutter) Name() string {
	return string(CutRepetitiveText)
}

// PrefixOverlapCutter 前缀重叠裁剪处理器
type PrefixOverlapCutter struct{ Cutter }

func (p *PrefixOverlapCutter) Process(ctx *PostprocessorContext) bool {
	// 补全内容前缀重复处理
	// 使用默认的cutLine参数值3
	processedCode := cutPrefixOverlap(ctx.CompletionCode, ctx.Prefix, ctx.Suffix, 3)
	if processedCode != ctx.CompletionCode {
		ctx.CompletionCode = processedCode
		return true
	}
	return false
}

func (p *PrefixOverlapCutter) Name() string {
	return string(CutPrefixOverlap)
}

// SuffixOverlapCutter 后缀重叠裁剪处理器
type SuffixOverlapCutter struct{ Cutter }

func (p *SuffixOverlapCutter) Process(ctx *PostprocessorContext) bool {
	// 使用默认的cutLine参数值3和ignoreOverlapLen参数值8
	processedCode := cutSuffixOverlap(ctx.CompletionCode, ctx.Prefix, ctx.Suffix, 3, 8)
	if processedCode != ctx.CompletionCode {
		ctx.CompletionCode = processedCode
		return true
	}
	return false
}

func (p *SuffixOverlapCutter) Name() string {
	return string(CutSuffixOverlap)
}

// 以下是工具函数的占位符，后续需要从common.py移植实现

// 无效括号丢弃处理器
type InvalidBracketsDiscarder struct{ Discarder }

func (p *InvalidBracketsDiscarder) Process(ctx *PostprocessorContext) bool {
	if !IsValidBrackets(ctx.CompletionCode) {
		return true
	}
	return false
}

func (p *InvalidBracketsDiscarder) Name() string {
	return string(DiscardInvalidBrackets)
}

// CSS内容丢弃
type CssContentDiscarder struct{ Discarder }

func (p *CssContentDiscarder) Process(ctx *PostprocessorContext) bool {
	// 如果是非CSS语言但是包含CSS内容，则去除CSS内容
	if strings.ToLower(ctx.Language) != "css" && JudgeCss(ctx.Language, ctx.CompletionCode, 0.7) {
		ctx.CompletionCode = ""
		return true
	}
	return false
}

func (p *CssContentDiscarder) Name() string {
	return string(DicardCssContent)
}

// SyntaxErrorDiscarder 语法错误丢弃处理器
type SyntaxErrorDiscarder struct{ Discarder }

func (p *SyntaxErrorDiscarder) Process(ctx *PostprocessorContext) bool {
	if !isCodeSyntax(ctx.Language, ctx.CompletionCode, ctx.Prefix, ctx.Suffix) {
		ctx.CompletionCode = ""
		return true
	}
	return false
}

func (p *SyntaxErrorDiscarder) Name() string {
	return string(DiscardSyntaxError)
}

// SyntaxErrorCutter 语法错误裁剪处理器
type SyntaxErrorCutter struct{ Cutter }

func (p *SyntaxErrorCutter) Process(ctx *PostprocessorContext) bool {
	// 使用 TreeSitter 进行语法错误拦截和代码裁剪
	tsUtil := NewTreeSitterUtil(ctx.Language)
	if tsUtil == nil {
		return false
	}

	processedCode := tsUtil.InterceptSyntaxErrorCode(ctx.CompletionCode, ctx.Prefix, ctx.Suffix)
	if processedCode != ctx.CompletionCode {
		ctx.CompletionCode = processedCode
		return true
	}
	return false
}

func (p *SyntaxErrorCutter) Name() string {
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
