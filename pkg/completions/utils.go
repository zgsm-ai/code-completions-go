package completions

import (
	"os"
	"regexp"
	"strings"
	"unicode"
)

// STR_PREFIX_CONFIG 字符串前缀配置
var STR_PREFIX_CONFIG = []struct {
	MaxTokenSequenceLength int
	LastTokensToConsider   int
}{
	{1, 10},
	{10, 30},
	{20, 45},
	{30, 60},
}

// computePrefixSuffixMatchLength 计算字符串的最长前缀后缀匹配长度
func computePrefixSuffixMatchLength(content string) []int {
	if len(content) == 0 {
		return []int{}
	}

	matchLengths := make([]int, len(content))
	matchLengths[0] = -1
	matchIndex := -1

	for i := 1; i < len(content); i++ {
		for matchIndex >= 0 && content[matchIndex+1] != content[i] {
			matchIndex = matchLengths[matchIndex]
		}
		if content[matchIndex+1] == content[i] {
			matchIndex++
		}
		matchLengths[i] = matchIndex
	}

	return matchLengths
}

// isRepetitiveContent 判断是否为重复内容
func isRepetitiveContent(content string) bool {
	matchLengths := computePrefixSuffixMatchLength(content)

	for _, config := range STR_PREFIX_CONFIG {
		// 考虑最后lastTokensToConsider个字符组成的字符串的前后缀重复字符数
		maxMatch := 0
		end := len(matchLengths)
		start := end - config.LastTokensToConsider
		if start < 0 {
			start = 0
		}

		for i := start; i < end; i++ {
			if matchLengths[i] > maxMatch {
				maxMatch = matchLengths[i]
			}
		}

		if len(content) >= config.LastTokensToConsider &&
			config.LastTokensToConsider-1-maxMatch <= config.MaxTokenSequenceLength {
			return true
		}
	}

	return false
}

// isRepetitive 判断字符串结尾是否为重复内容
func isRepetitive(content string) bool {
	// 反转字符串
	reversed := reverseString(content)
	if isRepetitiveContent(reversed) {
		return true
	}

	// 过滤空格后反转
	filtered := filterNonEmptyLines(content)
	reversedFiltered := reverseString(filtered)
	return isRepetitiveContent(reversedFiltered)
}

// reverseString 反转字符串
func reverseString(s string) string {
	runes := []rune(s)
	for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
		runes[i], runes[j] = runes[j], runes[i]
	}
	return string(runes)
}

// filterNonEmptyLines 过滤空行并反转
func filterNonEmptyLines(content string) string {
	lines := strings.Split(content, "\n")
	var nonEmptyLines []string
	for _, line := range lines {
		if strings.TrimSpace(line) != "" {
			nonEmptyLines = append(nonEmptyLines, line)
		}
	}
	return strings.Join(nonEmptyLines, "\n")
}

// containsOnlyNonAlpha 判断字符串是否仅包含非字母字符
func containsOnlyNonAlpha(input string) bool {
	for _, c := range input {
		if unicode.IsLetter(c) {
			return false
		}
	}
	return true
}

// checkContextIncludeText 验证text是否是上文结尾部分，以及是否是下文开头部分
func checkContextIncludeText(text, prefix, suffix string) bool {
	if strings.TrimSpace(text) == "" {
		return true
	}

	if len(text) <= 3 && strings.TrimSpace(suffix) != "" &&
		strings.HasPrefix(strings.TrimSpace(suffix), string(text[0])) {
		return true
	}

	if containsOnlyNonAlpha(text) && strings.TrimSpace(suffix) != "" &&
		strings.HasPrefix(strings.TrimSpace(suffix), string(text[0])) {
		return true
	}

	if text != "" && len(text) <= 5 {
		return false
	}

	trimmedText := strings.TrimSpace(text)
	trimmedPrefix := strings.TrimSpace(prefix)

	lineCount := len(strings.Split(text, "\n"))
	suffixLines := strings.Split(strings.TrimSpace(suffix), "\n")
	doubleTextLines := suffixLines
	if len(suffixLines) > lineCount*2 {
		doubleTextLines = suffixLines[:lineCount*2]
	}
	doubleText := strings.Join(doubleTextLines, "\n")

	if strings.HasSuffix(trimmedPrefix, trimmedText) {
		return true
	} else if strings.HasPrefix(doubleText, trimmedText) {
		return true
	} else if lineCount > 2 && strings.Contains(doubleText, trimmedText) {
		return true
	}

	return false
}

// cutSuffixOverlap 去除「补全内容」与suffix的前缀重叠部分
func cutSuffixOverlap(text, prefix, suffix string, cutLine int, ignoreOverlapLen int) string {
	if len(text) == 0 {
		return text
	}

	text = strings.TrimRight(text, " \t\n\r")
	textLen := len(text)
	suffix = strings.TrimSpace(suffix)

	// 循环多次，每次都截掉suffix的首行再进行内容重叠切割
	for i := 0; i < cutLine; i++ {
		suffixLines := strings.Split(suffix, "\n")
		suffixLen := len(suffix)

		if textLen == 0 || suffixLen == 0 {
			return text
		}

		firstLineSuffixLen := len(suffixLines[0])
		maxOverlapLength := min(textLen, suffixLen)

		for j := maxOverlapLength; j > maxOverlapLength/2; j-- {
			// 若suffix首行长度大于判重长度，则直接返回
			if j < firstLineSuffixLen {
				break
			}

			// 若suffix首行长度等于判重长度且首行仅有一个单词，那么无需判重直接返回
			if j == firstLineSuffixLen && len(strings.Split(suffix[:j], " ")) == 1 {
				break
			}

			// 一旦text和suffix存在重叠部分，立刻返回非重叠部分
			if text[textLen-j:] == suffix[:j] {
				// 避免误判切割到有效结束符，因此当需切割的内容长度小于等于某个值时，不进行重叠内容切割
				if len(strings.TrimSpace(suffix[:j])) > ignoreOverlapLen {
					return text[:textLen-j]
				}
			}
		}

		if len(suffixLines) > 1 {
			suffix = strings.Join(suffixLines[1:], "\n")
		} else {
			suffix = ""
		}
	}

	return text
}

// 去除「补全内容」与prefix的后缀重叠部分
func cutPrefixOverlap(text, prefix, suffix string, cutLine int) string {
	stripText := strings.TrimSpace(text)
	if len(stripText) == 0 {
		return text
	}

	splitText := strings.Split(stripText, "\n")
	if len(splitText) < 3 {
		if judgePrefixFullLineRepetitive(text, prefix) {
			return ""
		}
		return text
	}

	prefix = strings.TrimSpace(prefix)
	splitPrefix := strings.Split(prefix, "\n")

	matchLine := min(len(splitPrefix), len(splitText))
	patternTextList := splitText[:matchLine]

	for i := 0; i < cutLine; i++ {
		startIdx := len(splitPrefix) - matchLine - i
		if startIdx < 0 {
			startIdx = 0
		}
		endIdx := len(splitPrefix) - i

		curMatchTextList := splitPrefix[startIdx:endIdx]

		matchCount := 0
		continueFlag := true

		for j := 0; j < len(curMatchTextList) && j < len(patternTextList); j++ {
			if strings.TrimSpace(curMatchTextList[j]) == strings.TrimSpace(patternTextList[j]) {
				matchCount++
				if matchCount == 3 && continueFlag {
					return ""
				}
				if float64(matchCount)/float64(matchLine) >= 0.6 {
					return ""
				}
			} else {
				continueFlag = false
			}
		}
	}

	return text
}

// judgePrefixFullLineRepetitive 判断补全内容是否为与前缀内容尾行完全重复
func judgePrefixFullLineRepetitive(completionText, prefix string) bool {
	if len(prefix) == 0 || len(completionText) == 0 {
		return false
	}

	splitPrefixText := strings.Split(prefix, "\n")
	// 若将同行光标前的内容拼接到补全内容中，便于完全匹配
	linePrefixText := splitPrefixText[len(splitPrefixText)-1]
	completionText = linePrefixText + completionText

	splitCompletionText := strings.Split(completionText, "\n")
	var nonEmptyCompletionText []string
	for _, line := range splitCompletionText {
		if strings.TrimSpace(line) != "" {
			nonEmptyCompletionText = append(nonEmptyCompletionText, line)
		}
	}

	splitPrefixText = splitPrefixText[:len(splitPrefixText)-1]
	var nonEmptyPrefixText []string
	for _, line := range splitPrefixText {
		if strings.TrimSpace(line) != "" {
			nonEmptyPrefixText = append(nonEmptyPrefixText, line)
		}
	}

	// 若补全内容行数大于匹配内容行数，则直接返回False
	if len(nonEmptyCompletionText) > len(nonEmptyPrefixText) {
		return false
	}

	if len(nonEmptyPrefixText) >= len(nonEmptyCompletionText) {
		nonEmptyPrefixText = nonEmptyPrefixText[len(nonEmptyPrefixText)-len(nonEmptyCompletionText):]
	}

	for i := 0; i < len(nonEmptyCompletionText); i++ {
		if nonEmptyCompletionText[i] != nonEmptyPrefixText[i] {
			return false
		}
	}

	return true
}

// cutRepetitiveText 去除补全内容中的重复内容
func cutRepetitiveText(text string) string {
	if len(text) == 0 {
		return text
	}

	// 行数超过3才触发去重
	lineCount := len(strings.Split(strings.TrimSpace(text), "\n"))
	if lineCount < 3 {
		return text
	}

	return doCutRepetitiveText(text, 0.15)
}

// doCutRepetitiveText 若最长前后缀匹配长度占比超过阈值ratio，则去除补全内容中的重复内容
func doCutRepetitiveText(text string, ratio float64) string {
	if strings.TrimSpace(text) == "" {
		return text
	}

	// 计算text末尾有多少\n
	lastLineCount := 0
	for i := len(text) - 1; i >= 0; i-- {
		if text[i] == '\n' {
			lastLineCount++
		} else {
			break
		}
	}

	// 逆转补全文本
	reversedText := reverseString(strings.TrimRight(text, " \t\n\r"))

	// 计算当前逆转后的补全文本的最长前后缀长度
	matchLengths := computePrefixSuffixMatchLength(reversedText)
	maxMatchLengths := 0
	for _, length := range matchLengths {
		if length > maxMatchLengths {
			maxMatchLengths = length
		}
	}

	// 若最长前后缀长度/补全内容长度大于等于ratio则判断为重复内容
	if maxMatchLengths > 0 && float64(maxMatchLengths)/float64(len(reversedText)) >= ratio {
		if maxMatchLengths+1 < len(reversedText) {
			reversedText = reversedText[maxMatchLengths+1:]
		} else {
			reversedText = ""
		}
	}

	// 还原字符串
	result := reverseString(reversedText)

	// 还原\n
	for i := 0; i < lastLineCount; i++ {
		result += "\n"
	}

	return result
}

// longestCommonSubstring 计算两个字符串的最长公共子串
func longestCommonSubstring(a, b string) string {
	m, n := len(a), len(b)
	if m == 0 || n == 0 {
		return ""
	}

	prev := make([]int, n+1)
	maxLen := 0
	end := 0

	for i := 1; i <= m; i++ {
		current := make([]int, n+1)
		for j := 1; j <= n; j++ {
			if a[i-1] == b[j-1] {
				current[j] = prev[j-1] + 1
				if current[j] > maxLen {
					maxLen = current[j]
					end = i
				}
			}
		}
		prev = current
	}

	if maxLen == 0 {
		return ""
	}

	return a[end-maxLen : end]
}

// isExtremeRepetition 判断是否存在极端重复
func isExtremeRepetition(code string) (bool, string, int) {
	if len(code) == 0 {
		return false, "", 0
	}

	// 去除空行并获取非空行
	lines := strings.Split(code, "\n")
	var nonEmptyLines []string
	for _, line := range lines {
		trimmedLine := strings.TrimSpace(line)
		if trimmedLine != "" {
			nonEmptyLines = append(nonEmptyLines, trimmedLine)
		}
	}

	if len(nonEmptyLines) < 5 {
		return false, "", 0
	}

	n := len(nonEmptyLines)

	for i := 0; i < n-1; i++ {
		lcs := longestCommonSubstring(nonEmptyLines[i], nonEmptyLines[i+1])

		// 如果最长公共子串长度大于5且不小于行数的一半，则进行匹配过程
		if len(lcs) > 5 && len(lcs) >= len(nonEmptyLines[i])/2 {
			// 查找lcs在第一个字符串中的位置
			firstLineLcsIndex := strings.Index(nonEmptyLines[i], lcs)
			if firstLineLcsIndex == -1 {
				continue
			}

			// 统计重复次数
			count := 0
			for k := i + 1; k < n; k++ {
				if strings.Contains(nonEmptyLines[k], lcs) {
					if strings.Index(nonEmptyLines[k], lcs) == firstLineLcsIndex {
						count++
					}
				}
			}

			// 如果重复次数超过8或超过总行数的一半，则认为存在极端重复
			if count > 8 || count > n/2 {
				return true, lcs, count
			}
		}
	}

	return false, "", 0
}

// min 返回两个整数中的较小值
func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

// IsPythonText 判断是否为Python文本
func IsPythonText(text string) bool {
	pythonTextRules := os.Getenv("PYTHON_TEXT_RULES")
	if pythonTextRules == "" {
		pythonTextRules = "return self.name"
	}
	rules := strings.Split(pythonTextRules, ",")
	for _, rule := range rules {
		if strings.Contains(text, rule) {
			return true
		}
	}
	return false
}

// CSS相关正则表达式
var (
	cssPropertyPattern          = regexp.MustCompile(`^\s*[a-zA-Z-]+\s*:\s*[^;]+;\s*$`)
	cssSelectorPattern          = regexp.MustCompile(`^\s*[.#]?[a-zA-Z0-9_-]+\s*\{`)
	cssCommentPattern           = regexp.MustCompile(`/\*.*?\*/`)
	multilineCssPropertyPattern = regexp.MustCompile(`^\s*[a-zA-Z-]+\s*:\s*[^;]+;\s*$`)
)

// FrontLanguageEnum 前端语言枚举
type FrontLanguageEnum string

const (
	FrontLanguageVue  FrontLanguageEnum = "vue"
	FrontLanguageHTML FrontLanguageEnum = "html"
	FrontLanguageTS   FrontLanguageEnum = "typescript"
	FrontLanguageCSS  FrontLanguageEnum = "css"
)

// GetValues 获取前端语言枚举值
func (e FrontLanguageEnum) GetValues() []FrontLanguageEnum {
	return []FrontLanguageEnum{FrontLanguageVue, FrontLanguageHTML, FrontLanguageTS, FrontLanguageCSS}
}

// JudgeCss 判断文本是否为css样式
func JudgeCss(language string, text string, ratio float64) bool {
	// 检查语言是否为前端语言
	isFrontLanguage := false
	frontLanguages := FrontLanguageEnum("").GetValues()
	for _, lang := range frontLanguages {
		if strings.ToLower(language) == string(lang) {
			isFrontLanguage = true
			break
		}
	}

	if !isFrontLanguage {
		return false
	}

	if strings.Count(text, "\n") == 0 {
		return false
	}

	// 判断是否为多行CSS属性
	if multilineCssPropertyPattern.MatchString(text) {
		return true
	}

	count := 0
	lineCount := 0
	// 判断单行是否为CSS属性
	for _, line := range strings.Split(text, "\n") {
		if line == "\n" || line == "" {
			continue
		}
		if IncludeCss(line) {
			count++
		}
		lineCount++
	}

	if lineCount > 0 && float64(count)/float64(lineCount) > ratio {
		return true
	}
	return false
}

// IncludeCss 包含css样式
func IncludeCss(line string) bool {
	// 去除CSS注释
	line = cssCommentPattern.ReplaceAllString(line, "")

	// 检查是否包含CSS属性
	if cssPropertyPattern.MatchString(line) {
		return true
	}

	// 检查是否包含CSS选择器
	if cssSelectorPattern.MatchString(line) {
		return true
	}

	return false
}

// IsValidBrackets 用于判断text字符串中括号是否完整
func IsValidBrackets(text string) bool {
	stack := make([]rune, 0)
	mapping := map[rune]rune{
		')': '(',
		'}': '{',
		']': '[',
	}

	for _, char := range text {
		if closer, ok := mapping[char]; ok {
			if len(stack) == 0 || stack[len(stack)-1] != closer {
				return false
			}
			stack = stack[:len(stack)-1]
		} else if strings.ContainsRune("({[", char) {
			stack = append(stack, char)
		}
	}

	return len(stack) == 0
}

// GetLeftPairedSymbols 获取左括号映射
func GetLeftPairedSymbols() map[string]string {
	return map[string]string{
		"(": ")",
		"[": "]",
		"{": "}",
	}
}

// GetRightPairedSymbols 获取右括号映射
func GetRightPairedSymbols() map[string]string {
	rightSymbols := make(map[string]string)
	leftSymbols := GetLeftPairedSymbols()
	for k, v := range leftSymbols {
		rightSymbols[v] = k
	}
	return rightSymbols
}

// GetQuotesSymbols 获取引号符号映射
func GetQuotesSymbols() map[string]string {
	return map[string]string{
		"\"": "\"",
		"'":  "'",
	}
}

// GetBoundarySymbols 获取边界符号
func GetBoundarySymbols() []string {
	return []string{"{", "(", "[", "\"", "'", ":", "<", ";", ",", ">", ".", "`"}
}

// CountPairedSymbols 统计成对出现的符号
func CountPairedSymbols(text string) map[string]int {
	symbolsMap := make(map[string]int)
	rightSymbols := GetRightPairedSymbols()

	for _, char := range text {
		charStr := string(char)
		// 忽略孤立的右括号
		if expectedLeft, ok := rightSymbols[charStr]; ok {
			if symbolsMap[expectedLeft] == 0 {
				continue
			}
		}

		leftSymbols := GetLeftPairedSymbols()
		if _, ok := leftSymbols[charStr]; ok {
			symbolsMap[charStr]++
		}
	}

	return symbolsMap
}

// RemoveStrings 删除代码行中包含字符串的部分
func RemoveStrings(codeLine string) string {
	result := make([]rune, 0)
	inString := false
	var quoteChar rune
	buffer := make([]rune, 0)

	quotesSymbols := GetQuotesSymbols()

	for _, char := range codeLine {
		if !inString {
			if _, ok := quotesSymbols[string(char)]; ok {
				inString = true
				quoteChar = char
			} else {
				result = append(result, char)
			}
		} else {
			if char == quoteChar {
				inString = false
				quoteChar = 0
				buffer = make([]rune, 0)
			} else {
				buffer = append(buffer, char)
			}
		}
	}

	if inString {
		result = append(result, buffer...)
	}

	return string(result)
}

// IsCursorInParentheses 判断光标是否在括号内
func IsCursorInParentheses(prefix, suffix string) bool {
	findParenthesis := func(text string, symbol string, isReverse bool) int {
		count := 0
		leftSymbols := GetLeftPairedSymbols()

		if isReverse {
			runes := []rune(text)
			for i := len(runes) - 1; i >= 0; i-- {
				if string(runes[i]) == leftSymbols[symbol] {
					count++
				}
				if string(runes[i]) == symbol {
					if count <= 0 {
						return len(runes) - 1 - i
					}
					count--
				}
			}
		} else {
			for i, char := range text {
				if string(char) == leftSymbols[symbol] {
					count++
				}
				if string(char) == symbol {
					if count <= 0 {
						return i
					}
					count--
				}
			}
		}
		return -1
	}

	leftSymbols := GetLeftPairedSymbols()
	for leftSymbol, rightSymbol := range leftSymbols {
		leftParenthesisIndex := findParenthesis(prefix, leftSymbol, true)
		rightParenthesisIndex := findParenthesis(suffix, rightSymbol, false)
		if leftParenthesisIndex != -1 && rightParenthesisIndex != -1 {
			return true
		}
	}

	return false
}

// IsCursorInString 判断光标是否在字符串内
func IsCursorInString(cursorPrefix string) bool {
	quotesSymbols := GetQuotesSymbols()
	quotesCount := make(map[string]int)

	for k := range quotesSymbols {
		quotesCount[k] = 0
	}

	i := 0
	runes := []rune(cursorPrefix)
	for i < len(runes) {
		if runes[i] == '\\' {
			i += 2
			continue
		}

		charStr := string(runes[i])
		if _, ok := quotesSymbols[charStr]; ok {
			quotesCount[charStr]++
		}
		i++
	}

	// 如果引号数量为奇数，则光标在字符串内
	for _, count := range quotesCount {
		if count%2 == 1 {
			return true
		}
	}

	return false
}
