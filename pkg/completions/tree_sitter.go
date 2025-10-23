package completions

import (
	"fmt"
	"strings"
	"time"

	sitter "github.com/tree-sitter/go-tree-sitter"
	"go.uber.org/zap"
)

// TreeSitterUtil TreeSitter工具结构体
type TreeSitterUtil struct {
	language string
	parser   *sitter.Parser
	lang     *sitter.Language
	logger   *zap.Logger
}

// NewTreeSitterUtil 创建TreeSitter工具实例
func NewTreeSitterUtil(language string) *TreeSitterUtil {
	ts := &TreeSitterUtil{
		language: language,
		logger:   zap.L().With(zap.String("component", "TreeSitterUtil")),
	}

	// 暂时使用基本的语法检查，后续可以扩展为真正的tree-sitter解析
	// 初始化parser
	ts.parser = sitter.NewParser()

	// TODO: 这里需要根据不同的语言加载对应的tree-sitter语法
	// 由于Go版本的tree-sitter语言绑定比较复杂，暂时使用基本的语法检查
	// 后续可以通过CGO或者外部库来加载真正的tree-sitter语言解析器

	return ts
}

// calculateTime 计算函数执行时间的装饰器
func (t *TreeSitterUtil) calculateTime(funcName string, start time.Time) {
	duration := time.Since(start)
	t.logger.Info(fmt.Sprintf("函数 %s 的执行耗时为: %v", funcName, duration))
}

// IsCodeSyntax 检查代码语法
func (t *TreeSitterUtil) IsCodeSyntax(code string) bool {
	// 如果没有真正的tree-sitter语言解析器，使用基本的语法检查
	if t.lang == nil {
		return t.basicSyntaxCheck(code)
	}

	// 将代码转换为[]byte
	codeBytes := []byte(code)

	// 解析代码
	tree := t.parser.Parse(codeBytes, nil)
	if tree == nil {
		t.logger.Warn("解析代码失败", zap.String("language", t.language))
		return false
	}
	defer tree.Close()

	rootNode := tree.RootNode()
	hasError := rootNode.HasError()

	// 对于Lua语言，需要执行两次才能识别正确（根据Python版本的注释）
	if strings.ToLower(t.language) == "lua" {
		tree2 := t.parser.Parse(codeBytes, nil)
		if tree2 != nil {
			defer tree2.Close()
			rootNode2 := tree2.RootNode()
			hasError = rootNode2.HasError()
		}
	}

	return !hasError
}

// basicSyntaxCheck 基本的语法检查
func (t *TreeSitterUtil) basicSyntaxCheck(code string) bool {
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
func (t *TreeSitterUtil) InterceptSyntaxErrorCode(choicesText, prefix, suffix string) string {
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
func (t *TreeSitterUtil) interceptPythonSyntaxError(choicesText, prefix, suffix string) string {
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
func (t *TreeSitterUtil) interceptJavaScriptSyntaxError(choicesText, prefix, suffix string) string {
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
func (t *TreeSitterUtil) checkPythonSyntax(code string) bool {
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
func (t *TreeSitterUtil) checkJavaScriptSyntax(code string) bool {
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
func (t *TreeSitterUtil) checkGoSyntax(code string) bool {
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
func (t *TreeSitterUtil) ExtractBlockPrefixSuffix(choicesText, prefix, suffix string) (string, string) {
	// 简化实现：基于基本的代码块规则提取前后缀
	// 在实际应用中，这里应该使用TreeSitter进行真正的语法分析
	return t.extractSimpleBlockPrefixSuffix(choicesText, prefix, suffix)
}

// extractSimpleBlockPrefixSuffix 简化的代码块前后缀提取
func (t *TreeSitterUtil) extractSimpleBlockPrefixSuffix(choicesText, prefix, suffix string) (string, string) {
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
func (t *TreeSitterUtil) ExtractAccurateBlockPrefixSuffix(prefix, suffix string) (string, string) {
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
func (t *TreeSitterUtil) FindNearestBlock(code string, startNumber, endNumber int) string {
	// 如果没有真正的tree-sitter语言解析器，使用简化的实现
	if t.lang == nil {
		return t.findNearestBlockSimple(code, startNumber, endNumber)
	}

	// 使用tree-sitter进行真正的代码块查找
	codeBytes := []byte(code)
	tree := t.parser.Parse(codeBytes, nil)
	if tree == nil {
		t.logger.Warn("解析代码失败", zap.String("language", t.language))
		return code
	}
	defer tree.Close()

	rootNode := tree.RootNode()

	// 使用DFS遍历查找包含指定行号范围的完整block
	block := t.traverseForBlock(rootNode, startNumber, endNumber, 3)
	if block != nil {
		return t.GetNodeText(code, block)
	}

	return code
}

// findNearestBlockSimple 简化的代码块查找
func (t *TreeSitterUtil) findNearestBlockSimple(code string, startNumber, endNumber int) string {
	lines := strings.Split(code, "\n")
	if startNumber >= 0 && startNumber < len(lines) && endNumber >= 0 && endNumber < len(lines) {
		blockLines := lines[startNumber : endNumber+1]
		return strings.Join(blockLines, "\n")
	}
	return code
}

// traverseForBlock DFS遍历查找包含指定行号范围的完整block
func (t *TreeSitterUtil) traverseForBlock(node *sitter.Node, startNumber, endNumber int, depth int) *sitter.Node {
	if depth == 0 {
		return nil
	}

	// 获取节点的起始和结束位置
	startPoint := node.StartPosition()
	endPoint := node.EndPosition()

	// 检查当前节点是否包含指定的行号范围
	if startPoint.Row <= uint(startNumber) && endPoint.Row >= uint(endNumber) {
		// 遍历子节点
		childCount := node.ChildCount()
		for i := uint(0); i < childCount; i++ {
			child := node.Child(i)
			if child == nil {
				continue
			}

			result := t.traverseForBlock(child, startNumber, endNumber, depth-1)
			if result != nil {
				return result
			}
		}
		return node
	}

	return nil
}

// FindSecondLevelNodeByLineNum 按行号查找第二层节点
func (t *TreeSitterUtil) FindSecondLevelNodeByLineNum(code string, lineNum int) string {
	// 如果没有真正的tree-sitter语言解析器，使用简化的实现
	if t.lang == nil {
		return t.findSecondLevelNodeByLineNumSimple(code, lineNum)
	}

	// 使用tree-sitter进行真正的节点查找
	codeBytes := []byte(code)
	tree := t.parser.Parse(codeBytes, nil)
	if tree == nil {
		t.logger.Warn("解析代码失败", zap.String("language", t.language))
		return ""
	}
	defer tree.Close()

	rootNode := tree.RootNode()

	// 遍历第二层节点
	childCount := rootNode.ChildCount()
	for i := uint(0); i < childCount; i++ {
		child := rootNode.Child(i)
		if child == nil {
			continue
		}

		startPoint := child.StartPosition()
		endPoint := child.EndPosition()

		// 检查子节点是否包含指定行号
		if startPoint.Row <= uint(lineNum) && endPoint.Row >= uint(lineNum) {
			return t.GetNodeText(code, child)
		}
	}

	return ""
}

// findSecondLevelNodeByLineNumSimple 简化的第二层节点查找
func (t *TreeSitterUtil) findSecondLevelNodeByLineNumSimple(code string, lineNum int) string {
	lines := strings.Split(code, "\n")
	if lineNum >= 0 && lineNum < len(lines) {
		return lines[lineNum]
	}
	return ""
}

// FindSecondLevelNearestNodeByLineNum 查找指定行号的最近节点
func (t *TreeSitterUtil) FindSecondLevelNearestNodeByLineNum(code string, lineNum int) (string, string) {
	// 如果没有真正的tree-sitter语言解析器，使用简化的实现
	if t.lang == nil {
		return t.findSecondLevelNearestNodeByLineNumSimple(code, lineNum)
	}

	// 使用tree-sitter进行真正的节点查找
	codeBytes := []byte(code)
	tree := t.parser.Parse(codeBytes, nil)
	if tree == nil {
		t.logger.Warn("解析代码失败", zap.String("language", t.language))
		return "", ""
	}
	defer tree.Close()

	rootNode := tree.RootNode()

	var prefixNode, suffixNode *sitter.Node

	// 遍历第二层节点，找到前一个和后一个节点
	childCount := rootNode.ChildCount()
	for i := uint(0); i < childCount; i++ {
		child := rootNode.Child(i)
		if child == nil || child.HasError() {
			continue
		}

		endPoint := child.EndPosition()
		startPoint := child.StartPosition()

		// 找到前一个节点（在指定行号之前的节点）
		if endPoint.Row < uint(lineNum) {
			prefixNode = child
		}

		// 找到后一个节点（在指定行号之后的节点）
		if startPoint.Row > uint(lineNum) {
			suffixNode = child
			break // 找到第一个后置节点就停止
		}
	}

	// 提取节点文本
	var prefixText, suffixText string
	if prefixNode != nil {
		prefixText = t.GetNodeText(code, prefixNode)
	}
	if suffixNode != nil {
		suffixText = t.GetNodeText(code, suffixNode)
	}

	return prefixText, suffixText
}

// findSecondLevelNearestNodeByLineNumSimple 简化的最近节点查找
func (t *TreeSitterUtil) findSecondLevelNearestNodeByLineNumSimple(code string, lineNum int) (string, string) {
	lines := strings.Split(code, "\n")
	var prefixNode, suffixNode string

	if lineNum > 0 && lineNum < len(lines) {
		prefixNode = strings.Join(lines[:lineNum], "\n")
		suffixNode = strings.Join(lines[lineNum+1:], "\n")
	}

	return prefixNode, suffixNode
}

// GetLastKLineStrLen 获取代码最后k行字符串长度
func (t *TreeSitterUtil) GetLastKLineStrLen(code string, k int) int {
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

// GetNodeText 获取节点的文本内容
func (t *TreeSitterUtil) GetNodeText(sourceCode string, node *sitter.Node) string {
	if node == nil {
		return ""
	}

	// 将源代码转换为[]byte
	sourceBytes := []byte(sourceCode)

	// 获取节点的起始和结束字节位置
	startByte := node.StartByte()
	endByte := node.EndByte()

	// 确保字节范围在有效范围内
	if startByte < 0 || int(endByte) > len(sourceBytes) || startByte > endByte {
		return ""
	}

	// 提取节点文本
	nodeText := sourceBytes[startByte:endByte]
	return string(nodeText)
}

// GetText 获取节点的文本内容（简化版本）
func (t *TreeSitterUtil) GetText(node *sitter.Node) string {
	if node == nil {
		return ""
	}

	// 由于tree-sitter Go库可能没有Text方法，我们使用一个简化的实现
	// 在实际使用中，可能需要通过源代码和节点位置来提取文本
	return "node_text_placeholder"
}

// max 返回两个整数中的较大值
func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}
