package completions

import (
	"code-completion/pkg/model"
	"strings"
)

// 准备prompt，处理前后缀长度并拼接
func (h *CompletionHandler) preparePrompt(modelInfo *model.OpenAIModel, prefix, suffix, codeContext string) (*PromptResult, error) {
	// 处理前缀和可选的代码上下文
	newPrefix, newCodeContext := h.handlePrompt(prefix, true, codeContext, h.model.Config.MaxPrefixContext)
	// 处理后缀
	newSuffix, _ := h.handlePrompt(suffix, false, "", 0)

	// 拼接prompt模板
	prompt := h.getPromptTemplate(newPrefix, newSuffix, newCodeContext, &modelInfo.Config)

	// 计算prompt的token数量
	promptTokens := h.getTokensCount(prompt)

	return &PromptResult{
		Prompt:       prompt,
		PromptTokens: promptTokens,
		NewPrefix:    newPrefix,
		NewSuffix:    newSuffix,
	}, nil
}

// 处理prompt截断逻辑
func (h *CompletionHandler) handlePrompt(prompt string, isPrefix bool, optionalPrompt string, minPromptToken int) (string, string) {
	if h.model.Tokenizer == nil {
		return prompt, optionalPrompt
	}

	// 获取token数量
	promptTokens := h.model.Tokenizer.Encode(prompt, false)
	requestedTokensNum := len(promptTokens)

	var optionalTokensNum int
	var optionalToken []uint32
	if optionalPrompt != "" {
		optionalToken = h.model.Tokenizer.Encode(optionalPrompt, false)
		optionalTokensNum = len(optionalToken)
	}

	// 获取最大模型长度限制
	var maxModelLen int
	if isPrefix {
		maxModelLen = h.model.Config.MaxPrefixContext
	} else {
		maxModelLen = h.model.Config.MaxSuffixContext
	}

	// 如果总token数超过限制，需要截断
	if requestedTokensNum+optionalTokensNum > maxModelLen {
		needCutTokens := requestedTokensNum + optionalTokensNum - maxModelLen

		var truncatedTokens []uint32
		var truncatedOptionalTokens []uint32

		// 前缀截断逻辑
		if isPrefix {
			if requestedTokensNum-needCutTokens >= minPromptToken {
				// 直接截断prompt
				truncatedTokens = promptTokens[needCutTokens:]
			} else {
				// 保留最小token数，然后截断optionalPrompt
				actualMin := min(minPromptToken, requestedTokensNum)
				truncatedTokens = promptTokens[requestedTokensNum-actualMin:]
				if maxModelLen-actualMin > 0 && len(optionalToken) > maxModelLen-actualMin {
					truncatedOptionalTokens = optionalToken[:maxModelLen-actualMin]
				} else {
					truncatedOptionalTokens = optionalToken
				}
			}
		} else {
			// 后缀截断逻辑
			if requestedTokensNum > needCutTokens {
				truncatedTokens = promptTokens[:requestedTokensNum-needCutTokens]
			} else {
				truncatedTokens = []uint32{}
			}
		}

		// 将截断的tokens解码回字符串
		if len(truncatedTokens) > 0 {
			prompt = h.model.Tokenizer.Decode(truncatedTokens, false)
		}

		if len(truncatedOptionalTokens) > 0 {
			optionalPrompt = h.model.Tokenizer.Decode(truncatedOptionalTokens, false)
		}

		// 保证切割处为完整行
		prompt = h.ensureCompleteLines(prompt, isPrefix)
	}

	return prompt, optionalPrompt
}

// 确保切割处为完整行
func (h *CompletionHandler) ensureCompleteLines(prompt string, isPrefix bool) string {
	lines := strings.SplitAfter(prompt, "\n")
	if len(lines) > 0 {
		if isPrefix {
			// 前缀处理首行
			if !strings.HasPrefix(lines[0], "\n") && !strings.HasPrefix(lines[0], "\r\n") {
				lines = lines[1:]
			}
		} else {
			// 后缀处理尾行
			if len(lines) > 1 && !strings.HasSuffix(lines[len(lines)-1], "\n") {
				lines = lines[:len(lines)-1]
			}
		}
		return strings.Join(lines, "")
	}
	return prompt
}

// 判断是否为单行补全
func (h *CompletionHandler) judgeSingleCompletion(linePrefix, lineSuffix, language string) bool {
	// 简化的单行补全判断逻辑
	// 可以根据实际需求扩展，参考Python代码中的CompletionLineHandler逻辑

	// 如果光标前缀不为空且光标后缀为空，可能是单行补全
	if linePrefix != "" && lineSuffix == "" {
		return true
	}

	// 如果光标前缀以特定字符结尾，可能是单行补全
	if strings.HasSuffix(linePrefix, ".") ||
		strings.HasSuffix(linePrefix, " ") ||
		strings.HasSuffix(linePrefix, "\t") {
		return true
	}

	// 根据语言类型判断
	switch strings.ToLower(language) {
	case "python", "javascript", "typescript", "java", "c", "cpp":
		// 对于这些语言，如果光标在行首，可能是单行补全
		if strings.TrimSpace(linePrefix) == "" {
			return true
		}
	}

	return false
}

func (h *CompletionHandler) getTokensCount(prompt string) int {
	return h.model.Tokenizer.GetTokenCount(prompt)
}
