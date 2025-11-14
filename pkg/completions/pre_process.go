package completions

import (
	"code-completion/pkg/config"
	"strings"

	"go.uber.org/zap"
)

/**
 * 截断超长的提示词(前缀，后缀，上下文)
 * 优先保留最靠近补全位置的代码
 */
func (h *CompletionHandler) truncatePrompt(cfg *config.ModelConfig, ppt *PromptOptions) {
	tokenizer := h.llm.Tokenizer()
	if tokenizer == nil {
		return
	}

	prefixTokens := tokenizer.Encode(ppt.Prefix)
	prefixTokensNum := len(prefixTokens)

	suffixTokens := tokenizer.Encode(ppt.Suffix)
	suffixTokensNum := len(suffixTokens)

	contextTokens := tokenizer.Encode(ppt.CodeContext)
	contextTokensNum := len(contextTokens)

	// 获取最大模型长度限制
	prefixMax := h.llm.Config().MaxPrefixContext
	suffixMax := h.llm.Config().MaxSuffixContext

	// 如果总token数超过限制，需要截断
	if prefixTokensNum+contextTokensNum > prefixMax {
		needCutTokens := prefixTokensNum + contextTokensNum - prefixMax

		// 前缀都已经超长了，就把上下文完全丢弃掉
		if prefixTokensNum >= prefixMax {
			prefixTokens = prefixTokens[prefixTokensNum-prefixMax:]
			ppt.CodeContext = ""
			ppt.Prefix = tokenizer.Decode(prefixTokens)
			ppt.Prefix = h.trimFirstLine(ppt.Prefix)
		} else {
			contextTokens = contextTokens[needCutTokens:]
			ppt.CodeContext = tokenizer.Decode(contextTokens)
		}
	}
	if suffixTokensNum > suffixMax {
		suffixTokens = suffixTokens[:suffixMax]
		ppt.Suffix = tokenizer.Decode(suffixTokens)
		ppt.Suffix = h.trimLastLine(ppt.Suffix)
	}
}

func (h *CompletionHandler) trimFirstLine(prompt string) string {
	lines := strings.SplitAfter(prompt, "\n")
	if len(lines) > 0 {
		if !strings.HasPrefix(lines[0], "\n") && !strings.HasPrefix(lines[0], "\r\n") {
			lines = lines[1:]
		}
		return strings.Join(lines, "")
	}
	return prompt
}

func (h *CompletionHandler) trimLastLine(suffix string) string {
	lines := strings.SplitAfter(suffix, "\n")
	if len(lines) > 0 {
		if len(lines) > 1 && !strings.HasSuffix(lines[len(lines)-1], "\n") {
			lines = lines[:len(lines)-1]
		}
		return strings.Join(lines, "")
	}
	return suffix
}

func (h *CompletionHandler) getTokensCount(prompt string) int {
	return h.llm.Tokenizer().GetTokenCount(prompt)
}

/**
 * 获取加了FIM标记的prompt文本
 */
func (h *CompletionHandler) getFimPrompt(prefix, suffix, codeContext string, cfg *config.ModelConfig) string {
	return cfg.FimBegin + codeContext + "\n" + prefix + cfg.FimHole + suffix + cfg.FimEnd
}

/**
 * 准备停用词
 */
func (h *CompletionHandler) prepareStopWords(input *CompletionInput) []string {
	var stopWords []string

	// 添加请求中的停用词
	if len(input.Stop) > 0 {
		stopWords = append(stopWords, input.Stop...)
	}

	// 添加默认的FIM停用词
	stopWords = append(stopWords, "<｜end▁of▁sentence｜>")

	// 如果后缀为空，添加系统停用词
	if input.Processed.Suffix == "" || strings.TrimSpace(input.Processed.Suffix) == "" {
		stopWords = append(stopWords, "\n\n", "\n\n\n")
	}

	return stopWords
}

/**
 *	修剪补全结果
 */
func (h *CompletionHandler) pruneCompletionCode(completionText, prefix, suffix, lang string) string {
	postprocessorContext := &PostprocessorContext{
		Language:       lang,
		CompletionCode: completionText,
		Prefix:         prefix,
		Suffix:         suffix,
	}
	var chain *PostprocessorChain
	var err error
	if len(h.cfg.CustomPruners) > 0 {
		chain, err = NewPostprocessorChainByNames(h.cfg.CustomPruners)
		if err != nil {
			zap.L().Error("Invalid config: 'customPruners'",
				zap.Any("customPruners", h.cfg.CustomPruners))
		}
	}
	if chain == nil {
		chain = NewDefaultPostprocessorChain()
	}
	if chain.Process(postprocessorContext) {
		zap.L().Debug("Prune by Postprocessors",
			zap.String("pre", completionText),
			zap.String("post", postprocessorContext.CompletionCode),
			zap.Any("hits", chain.GetHitProcessors()))
	}
	return postprocessorContext.CompletionCode
}
