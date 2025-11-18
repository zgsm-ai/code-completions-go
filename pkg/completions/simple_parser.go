package completions

import (
	"strings"

	"go.uber.org/zap"
)

type SimpleParser struct {
	language string
	logger   *zap.Logger
}

func NewSimpleParser(language string) *SimpleParser {
	ts := &SimpleParser{
		language: language,
		logger:   zap.L().With(zap.String("component", "SimpleParser")),
	}
	return ts
}

// IsCodeSyntax 检查代码语法
func (t *SimpleParser) IsCodeSyntax(code string) bool {
	// 使用基本的语法检查
	return t.basicSyntaxCheck(code)
}

// basicSyntaxCheck 基本的语法检查
func (t *SimpleParser) basicSyntaxCheck(code string) bool {
	switch strings.ToLower(t.language) {
	case "python":
		return t.checkPythonSyntax(code)
	case "javascript", "typescript":
		return t.checkJavaScriptSyntax(code)
	case "go":
		return t.checkGoSyntax(code)
	default:
		return true // 对于不支持的语言，默认返回true
	}
}

/**
 * 拦截语法错误代码
 */
func (t *SimpleParser) InterceptSyntaxErrorCode(choicesText, prefix, suffix string) string {
	if choicesText == "" {
		return choicesText
	}

	cutCode := choicesText
	maxCutCount := t.GetLastKLineStrLen(cutCode, 1)

	tryCount := 0
	for i := 0; i < maxCutCount; i++ {
		tryCount++
		if t.IsCodeSyntax(prefix+cutCode+suffix) && strings.TrimSpace(cutCode) != "" {
			if i != 0 {
				t.logger.Info("切割代码成功",
					zap.String("language", t.language),
					zap.String("pre-trim", choicesText),
					zap.String("post-trim", cutCode))
			}
			return strings.TrimRight(cutCode, "\n\r\t ")
		}

		if len(cutCode) > 0 {
			cutCode = cutCode[:len(cutCode)-1]
		} else {
			break
		}
	}

	t.logger.Warn("切割代码失败", zap.String("language", t.language), zap.String("code", choicesText))
	return choicesText
}

// interceptPythonSyntaxError 拦截Python语法错误
func (t *SimpleParser) interceptPythonSyntaxError(choicesText, prefix, suffix string) string {
	lines := strings.Split(choicesText, "\n")
	var validLines []string

	indentLevel := 0
	for _, line := range lines {
		trimmed := strings.TrimSpace(line)
		if trimmed == "" {
			validLines = append(validLines, line)
			continue
		}

		// 计算当前行的缩进级别
		currentIndent := 0
		for _, char := range line {
			if char == ' ' || char == '\t' {
				currentIndent++
			} else {
				break
			}
		}

		// 检查缩进是否合理
		if currentIndent > indentLevel+4 { // 突然增加太多缩进可能是错误的
			continue
		}

		// 更新缩进级别
		if strings.HasSuffix(trimmed, ":") {
			indentLevel = currentIndent + 4
		} else if currentIndent < indentLevel {
			indentLevel = currentIndent
		}

		validLines = append(validLines, line)
	}

	return strings.Join(validLines, "\n")
}

// interceptJavaScriptSyntaxError 拦截JavaScript语法错误
func (t *SimpleParser) interceptJavaScriptSyntaxError(choicesText, prefix, suffix string) string {
	lines := strings.Split(choicesText, "\n")
	var validLines []string

	bracketCount := 0
	for _, line := range lines {
		trimmed := strings.TrimSpace(line)
		if trimmed == "" {
			validLines = append(validLines, line)
			continue
		}

		// 检查括号匹配
		for _, char := range line {
			if char == '{' {
				bracketCount++
			} else if char == '}' {
				bracketCount--
				if bracketCount < 0 {
					bracketCount = 0
				}
			}
		}

		validLines = append(validLines, line)
	}

	// 如果括号不匹配，尝试修复
	if bracketCount > 0 {
		for i := 0; i < bracketCount; i++ {
			validLines = append(validLines, "}")
		}
	}

	return strings.Join(validLines, "\n")
}

// checkPythonSyntax 检查Python语法
func (t *SimpleParser) checkPythonSyntax(code string) bool {
	lines := strings.Split(code, "\n")
	indentStack := []int{0}

	for _, line := range lines {
		trimmed := strings.TrimSpace(line)
		if trimmed == "" {
			continue
		}

		// 计算当前行的缩进级别
		currentIndent := 0
		for _, char := range line {
			if char == ' ' || char == '\t' {
				currentIndent++
			} else {
				break
			}
		}

		// 检查缩进是否合理
		if currentIndent > indentStack[len(indentStack)-1] && !strings.HasSuffix(trimmed, ":") {
			return false
		}

		// 更新缩进栈
		if strings.HasSuffix(trimmed, ":") {
			indentStack = append(indentStack, currentIndent+4)
		} else {
			for len(indentStack) > 1 && currentIndent <= indentStack[len(indentStack)-2] {
				indentStack = indentStack[:len(indentStack)-1]
			}
		}
	}

	return true
}

// checkJavaScriptSyntax 检查JavaScript语法
func (t *SimpleParser) checkJavaScriptSyntax(code string) bool {
	bracketCount := 0
	parenCount := 0
	bracketSquareCount := 0

	for _, char := range code {
		switch char {
		case '{':
			bracketCount++
		case '}':
			bracketCount--
			if bracketCount < 0 {
				return false
			}
		case '(':
			parenCount++
		case ')':
			parenCount--
			if parenCount < 0 {
				return false
			}
		case '[':
			bracketSquareCount++
		case ']':
			bracketSquareCount--
			if bracketSquareCount < 0 {
				return false
			}
		}
	}

	return bracketCount == 0 && parenCount == 0 && bracketSquareCount == 0
}

// checkGoSyntax 检查Go语法
func (t *SimpleParser) checkGoSyntax(code string) bool {
	bracketCount := 0
	parenCount := 0
	bracketSquareCount := 0

	for _, char := range code {
		switch char {
		case '{':
			bracketCount++
		case '}':
			bracketCount--
			if bracketCount < 0 {
				return false
			}
		case '(':
			parenCount++
		case ')':
			parenCount--
			if parenCount < 0 {
				return false
			}
		case '[':
			bracketSquareCount++
		case ']':
			bracketSquareCount--
			if bracketSquareCount < 0 {
				return false
			}
		}
	}

	return bracketCount == 0 && parenCount == 0 && bracketSquareCount == 0
}

// ExtractBlockPrefixSuffix 提取代码块前后缀
func (t *SimpleParser) ExtractBlockPrefixSuffix(choicesText, prefix, suffix string) (string, string) {
	// 简化实现：基于基本的代码块规则提取前后缀
	// 在实际应用中，这里应该使用TreeSitter进行真正的语法分析
	return t.extractSimpleBlockPrefixSuffix(choicesText, prefix, suffix)
}

// extractSimpleBlockPrefixSuffix 简化的代码块前后缀提取
func (t *SimpleParser) extractSimpleBlockPrefixSuffix(choicesText, prefix, suffix string) (string, string) {
	const specialMiddleSignal = "<special-middle>"
	code := prefix + specialMiddleSignal + choicesText + specialMiddleSignal + suffix

	startNumber, endNumber := getChoicesTextLineNumber(code, specialMiddleSignal)

	// 简化实现：基于行号提取代码块
	lines := strings.Split(code, "\n")
	if startNumber >= 0 && startNumber < len(lines) && endNumber >= 0 && endNumber < len(lines) {
		blockLines := lines[startNumber : endNumber+1]
		blockCode := strings.Join(blockLines, "\n")
		return isolatedPrefixSuffix(blockCode, specialMiddleSignal)
	}

	return prefix, suffix
}

// ExtractAccurateBlockPrefixSuffix 提取准确的代码块前后缀
func (t *SimpleParser) ExtractAccurateBlockPrefixSuffix(prefix, suffix string) (string, string) {
	const specialMiddleSignal = "<special-middle>"
	code := prefix + specialMiddleSignal + suffix
	lineNum, _ := getChoicesTextLineNumber(code, specialMiddleSignal)

	// 简化实现：基于行号提取代码块
	lines := strings.Split(code, "\n")
	if lineNum >= 0 && lineNum < len(lines) {
		// 提取当前行所在的代码块
		startLine := max(0, lineNum-2)
		endLine := min(len(lines), lineNum+3)

		blockLines := lines[startLine:endLine]
		blockCode := strings.Join(blockLines, "\n")
		return isolatedPrefixSuffix(blockCode, specialMiddleSignal)
	}

	return prefix, suffix
}

// FindNearestBlock 查找最近的代码块
func (t *SimpleParser) FindNearestBlock(code string, startNumber, endNumber int) string {
	// 使用简化的实现
	return t.findNearestBlockSimple(code, startNumber, endNumber)
}

// findNearestBlockSimple 简化的代码块查找
func (t *SimpleParser) findNearestBlockSimple(code string, startNumber, endNumber int) string {
	lines := strings.Split(code, "\n")
	if startNumber >= 0 && startNumber < len(lines) && endNumber >= 0 && endNumber < len(lines) {
		blockLines := lines[startNumber : endNumber+1]
		return strings.Join(blockLines, "\n")
	}
	return code
}

// FindSecondLevelNodeByLineNum 按行号查找第二层节点
func (t *SimpleParser) FindSecondLevelNodeByLineNum(code string, lineNum int) string {
	// 使用简化的实现
	return t.findSecondLevelNodeByLineNumSimple(code, lineNum)
}

// findSecondLevelNodeByLineNumSimple 简化的第二层节点查找
func (t *SimpleParser) findSecondLevelNodeByLineNumSimple(code string, lineNum int) string {
	lines := strings.Split(code, "\n")
	if lineNum >= 0 && lineNum < len(lines) {
		return lines[lineNum]
	}
	return ""
}

// FindSecondLevelNearestNodeByLineNum 查找指定行号的最近节点
func (t *SimpleParser) FindSecondLevelNearestNodeByLineNum(code string, lineNum int) (string, string) {
	// 使用简化的实现
	return t.findSecondLevelNearestNodeByLineNumSimple(code, lineNum)
}

// findSecondLevelNearestNodeByLineNumSimple 简化的最近节点查找
func (t *SimpleParser) findSecondLevelNearestNodeByLineNumSimple(code string, lineNum int) (string, string) {
	lines := strings.Split(code, "\n")
	var prefixNode, suffixNode string

	if lineNum > 0 && lineNum < len(lines) {
		prefixNode = strings.Join(lines[:lineNum], "\n")
		suffixNode = strings.Join(lines[lineNum+1:], "\n")
	}

	return prefixNode, suffixNode
}

// GetLastKLineStrLen 获取代码最后k行字符串长度
func (t *SimpleParser) GetLastKLineStrLen(code string, k int) int {
	lines := strings.Split(code, "\n")
	var lastKLines []string

	for i := len(lines) - 1; i >= 0; i-- {
		if len(lines[i]) == 0 {
			continue
		}
		if len(lastKLines) == k {
			break
		}
		lastKLines = append(lastKLines, lines[i])
	}

	totalLen := 0
	for _, line := range lastKLines {
		totalLen += len(line)
	}

	// 加上换行符的长度
	newlineCount := max(k-1, len(lastKLines)-1)
	return totalLen + newlineCount
}

// getChoicesTextLineNumber 获取补全内容在代码中的行号
func getChoicesTextLineNumber(code, pattern string) (int, int) {
	codeSplit := strings.Split(code, "\n")
	startNumber := 0
	endNumber := 0

	for i := range codeSplit {
		if strings.Contains(codeSplit[i], pattern) && startNumber == 0 {
			startNumber = i
			continue
		}

		if strings.Contains(codeSplit[i], pattern) && startNumber != 0 {
			endNumber = i
			break
		}
	}

	return startNumber, endNumber
}

// isolatedPrefixSuffix 分离前后缀
func isolatedPrefixSuffix(code, pattern string) (string, string) {
	if code == "" {
		return "", ""
	}

	split := strings.Split(code, pattern)
	if len(split) >= 2 {
		return split[0], split[len(split)-1]
	}

	return "", ""
}

// max 返回两个整数中的较大值
func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}
