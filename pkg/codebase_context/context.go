package codebase_context

import (
	"context"
	"fmt"
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

// RequestContext 请求上下文信息
func (c *ContextClient) RequestContext(ctx context.Context, clientID, codebasePath, filePath string,
	codeSnippets []string, queries []string, headers map[string]string) (*SearchResult, error) {

	if clientID == "" || codebasePath == "" || filePath == "" {
		return &SearchResult{}, nil
	}

	// 创建上下文，设置超时
	ctx, cancel := context.WithTimeout(ctx, config.ContextCostTime)
	defer cancel()

	var wg sync.WaitGroup
	var def_mu sync.Mutex
	var sen_mu sync.Mutex

	// 初始化结果数组
	definitionResults := make([]*ResponseData, len(codeSnippets))
	semanticResults := make([]*ResponseData, len(queries))

	// 定义检索
	if len(codeSnippets) > 0 && config.EnableDefinitionSearch {
		for i, codeSnippet := range codeSnippets {
			if codeSnippet != "" {
				wg.Add(1)
				go func(snippet string, idx int) {
					defer wg.Done()

					// 检查上下文是否已取消
					select {
					case <-ctx.Done():
						return
					default:
					}

					data, err := c.apiClient.SearchDefinition(ctx, clientID, codebasePath, filePath, snippet, nil, nil, headers)

					def_mu.Lock()
					if err != nil {
						zap.L().Warn("Definition search request failed", zap.Error(err))
					} else if data != nil {
						// 保证结束后,一定不会有数据写入
						select {
						case <-ctx.Done():
							def_mu.Unlock()
							return
						default:
							definitionResults[idx] = data
						}
					}
					def_mu.Unlock()
				}(codeSnippet, i)
			}
		}
	}

	// 语义检索
	if len(queries) > 0 && config.EnableSemanticSearch {
		for i, query := range queries {
			if query != "" {
				wg.Add(1)
				go func(q string, idx int) {
					defer wg.Done()

					// 检查上下文是否已取消
					select {
					case <-ctx.Done():
						return
					default:
					}

					data, err := c.apiClient.SearchSemantic(ctx, clientID, codebasePath, q, headers)

					sen_mu.Lock()
					if err != nil {
						zap.L().Warn("Semantic search request failed", zap.Error(err))
					} else if data != nil {
						select {
						case <-ctx.Done():
							sen_mu.Unlock()
							return
						default:
							semanticResults[idx] = data
						}

					}
					sen_mu.Unlock()
				}(query, i)
			}
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
	case <-done:
		// 所有请求完成
	case <-ctx.Done():
		// 上下文取消，直接返回已收集的结果
		zap.L().Warn("Context timeout, returning partial results")
	}

	return &SearchResult{
		DefinitionResults: definitionResults,
		SemanticResults:   semanticResults,
		RelationResults:   nil,
	}, nil
}

// GetContext 获取上下文信息
func (c *ContextClient) GetContext(ctx context.Context, clientID, projectPath, filePath, prefix, suffix, importContent string, headers map[string]string) (string, error) {
	if clientID == "" || projectPath == "" || filePath == "" || (prefix == "" && suffix == "") {
		return "", nil
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

	searchResult, err := c.RequestContext(ctx, clientID, projectPath, fullFilePath,
		definitionCodeSnaps, []string{semanticSearchContent}, headers)

	if err != nil {
		zap.L().Error("Failed to request context", zap.Error(err))
		return "", err
	}

	// 解析语义检索结果
	semanticCodes := parseSemantic(searchResult.SemanticResults)

	// 解析定义检索结果
	defCodes := parseDefinition(searchResult.DefinitionResults)

	// 解析关系检索结果
	// relationCodes := parseRelation(searchResult.RelationResults)

	var allCodes []string

	// 合并定义检索结果
	for _, item := range defCodes {
		if len(item) > 1 {
			allCodes = append(allCodes, item[1:]...)
		}
	}

	// 合并语义检索结果
	for _, item := range semanticCodes {
		if len(item) >= 2 {
			allCodes = append(allCodes, item[:2]...)
		}
	}

	// // 合并关系检索结果
	// if relationCodes != nil {
	// 	for _, item := range relationCodes {
	// 		if len(item) > 1 {
	// 			allCodes = append(allCodes, item[1:]...)
	// 		}
	// 	}
	// }

	// 合并所有结果
	semanticResult := strings.Join(allCodes, "\n")

	// 添加注释
	return getComment(fullFilePath, semanticResult), nil
}
