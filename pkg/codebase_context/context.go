package codebase_context

import (
	"context"
	"fmt"
	"net/http"
	"path/filepath"
	"strings"
	"sync"

	"go.uber.org/zap"
)

// ContextClient 上下文客户端
type ContextClient struct {
	apiClient *APIClient
}

// NewContextClient 创建新的上下文客户端
func NewContextClient() *ContextClient {
	return &ContextClient{
		apiClient: NewAPIClient(),
	}
}

// SearchResult 搜索结果
type SearchResult struct {
	DefinitionResults []*ResponseData
	SemanticResults   []*ResponseData
	RelationResults   []*ResponseData
}

func (c *ContextClient) searchDefinitionAsync(ctx context.Context, clientID, codebasePath, filePath, codeSnippet string,
	headers http.Header, wg *sync.WaitGroup, results []*ResponseData, idx int) {
	defer wg.Done()

	data, err := c.searchDefinition(ctx, clientID, codebasePath, filePath, codeSnippet, headers)
	if err != nil {
		results[idx] = data
	}
}

func (c *ContextClient) searchRelationAsync(ctx context.Context, clientID, codebasePath, filePath, codeSnippet string,
	headers http.Header, wg *sync.WaitGroup, results []*ResponseData, idx int) {
	defer wg.Done()

	data, err := c.searchRelation(ctx, clientID, codebasePath, filePath, codeSnippet, headers)
	if err != nil {
		results[idx] = data
	}
}

func (c *ContextClient) searchSemanticAsync(ctx context.Context, clientID, codebasePath, query string, headers http.Header,
	wg *sync.WaitGroup, results []*ResponseData, idx int) {
	defer wg.Done()

	data, err := c.searchSemantic(ctx, clientID, codebasePath, query, headers)
	if err != nil {
		results[idx] = data
	}
}

// 请求上下文信息
func (c *ContextClient) RequestContext(ctx context.Context, clientID, codebasePath, filePath string,
	codeSnippets []string, queries []string, headers http.Header) *SearchResult {
	if clientID == "" || codebasePath == "" || filePath == "" {
		return &SearchResult{}
	}

	// 创建上下文，设置超时
	ctx, cancel := context.WithTimeout(ctx, contextConfig.TotalTimeout)
	defer cancel()

	var wg sync.WaitGroup
	// 初始化结果数组
	definitionResults := make([]*ResponseData, len(codeSnippets))
	relationResults := make([]*ResponseData, len(codeSnippets))
	semanticResults := make([]*ResponseData, len(queries))

	// 定义检索
	if len(codeSnippets) > 0 && !contextConfig.DisableDefinitionSearch {
		for i, codeSnippet := range codeSnippets {
			if codeSnippet == "" {
				continue
			}
			wg.Add(1)
			go c.searchDefinitionAsync(ctx, clientID, codebasePath, filePath, codeSnippet, headers, &wg, definitionResults, i)
		}
	}
	// 调用链检索
	if len(codeSnippets) > 0 && !contextConfig.DisableRelationSearch {
		for i, codeSnippet := range codeSnippets {
			if codeSnippet == "" {
				continue
			}
			wg.Add(1)
			go c.searchRelationAsync(ctx, clientID, codebasePath, filePath, codeSnippet, headers, &wg, relationResults, i)
		}
	}

	// 语义检索
	if len(queries) > 0 && !contextConfig.DisableSemanticSearch {
		for i, query := range queries {
			if query == "" {
				continue
			}
			wg.Add(1)
			go c.searchSemanticAsync(ctx, clientID, codebasePath, query, headers, &wg, semanticResults, i)
		}
	}

	// 等待所有请求完成或上下文取消
	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	// 等待完成或上下文取消
	select {
	case <-done: // 所有请求完成
	case <-ctx.Done(): // 上下文取消，直接返回已收集的结果
		zap.L().Warn("Context timeout, returning partial results", zap.Error(ctx.Err()))
	}
	return &SearchResult{
		DefinitionResults: definitionResults,
		SemanticResults:   semanticResults,
		RelationResults:   relationResults,
	}
}

// 获取上下文信息
func (c *ContextClient) GetContext(ctx context.Context, clientID, projectPath, filePath, prefix, suffix, importContent string, headers http.Header) string {
	if clientID == "" || projectPath == "" || filePath == "" || (prefix == "" && suffix == "") {
		return ""
	}

	// 构建完整文件路径
	fullFilePath := filepath.Join(projectPath, filePath)

	// Windows路径处理
	if len(projectPath) > 1 && projectPath[1:3] == ":\\" {
		fullFilePath = strings.ReplaceAll(fullFilePath, "/", "\\")
	}

	// 获取语义搜索内容（前缀最后几行）
	semanticSearchContent := rSliceAfterNthInstance(prefix, "\n", 4)

	// 定义检索代码片段
	definitionCodeSnaps := []string{
		fmt.Sprintf("%s%s%s", importContent, prefix, suffix),
	}

	searchResult := c.RequestContext(ctx, clientID, projectPath, fullFilePath,
		definitionCodeSnaps, []string{semanticSearchContent}, headers)

	// 解析语义检索结果
	semanticCodes := parseSemantic(searchResult.SemanticResults)

	// 解析定义检索结果
	defCodes := parseDefinition(searchResult.DefinitionResults)

	// 解析关系检索结果
	relationCodes := parseRelation(searchResult.RelationResults)

	var allCodes []string

	// 合并定义检索结果
	for _, item := range defCodes {
		allCodes = append(allCodes, item.FilePath, item.Content)
		// if len(item) > 1 {
		// 	allCodes = append(allCodes, item[1:]...)
		// }
	}

	// 合并语义检索结果
	for _, item := range semanticCodes {
		allCodes = append(allCodes, item.FilePath, item.Content)
		// if len(item) >= 2 {
		// 	allCodes = append(allCodes, item[:2]...)
		// }
	}

	// 合并关系检索结果
	for _, item := range relationCodes {
		allCodes = append(allCodes, item.FilePath, item.Content)
		// if len(item) > 1 {
		// 	allCodes = append(allCodes, item[1:]...)
		// }
	}

	// 合并所有结果
	semanticResult := strings.Join(allCodes, "\n")

	// 添加注释
	return getComment(fullFilePath, semanticResult)
}

// 搜索代码定义
func (c *ContextClient) searchDefinition(ctx context.Context, clientID, codebasePath, filePath, codeSnippet string, headers http.Header) (*ResponseData, error) {
	params := RequestParam{
		ClientID:     clientID,
		CodebasePath: codebasePath,
		FilePath:     filePath,
		CodeSnippet:  codeSnippet,
	}

	return c.apiClient.DoRequest(ctx, contextConfig.CodebaseDefinitionURL, params, headers, "GET")
}

// 语义搜索
func (c *ContextClient) searchSemantic(ctx context.Context, clientID, codebasePath, query string, headers http.Header) (*ResponseData, error) {
	params := RequestParam{
		ClientID:       clientID,
		CodebasePath:   codebasePath,
		Query:          query,
		TopK:           contextConfig.SemanticTopK,
		ScoreThreshold: contextConfig.SemanticScoreThreshold,
	}

	return c.apiClient.DoRequest(ctx, contextConfig.CodebaseSemanticURL, params, headers, "POST")
}

// 关系检索
func (c *ContextClient) searchRelation(ctx context.Context, clientID, codebasePath, filePath, codeSnippet string, headers http.Header) (*ResponseData, error) {
	params := RequestParam{
		ClientID:       clientID,
		CodebasePath:   codebasePath,
		FilePath:       filePath,
		CodeSnippet:    codeSnippet,
		MaxLayer:       contextConfig.RelationLayer,
		IncludeContent: contextConfig.RelationIncludeContent,
	}

	return c.apiClient.DoRequest(ctx, contextConfig.CodebaseRelationURL, params, headers, "GET")
}
