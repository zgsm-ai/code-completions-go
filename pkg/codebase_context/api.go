package codebase_context

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"net/http"
	"time"

	"go.uber.org/zap"
)

// HTTPClient 接口定义
type HTTPClient interface {
	Do(req *http.Request) (*http.Response, error)
}

// APIClient API客户端结构体
type APIClient struct {
	client        HTTPClient
	definitionURL string
	semanticURL   string
	relationURL   string
	timeout       time.Duration
}

// NewAPIClient 创建新的API客户端
func NewAPIClient() *APIClient {
	return &APIClient{
		client: &http.Client{
			Timeout: config.RequestTimeout,
		},
		definitionURL: config.CodebaseDefinitionURL,
		semanticURL:   config.CodebaseSemanticURL,
		relationURL:   config.CodebaseRelationURL,
		timeout:       config.RequestTimeout,
	}
}

// RequestParam 请求参数
type RequestParam struct {
	ClientID       string  `json:"clientId"`
	CodebasePath   string  `json:"codebasePath"`
	FilePath       string  `json:"filePath,omitempty"`
	CodeSnippet    string  `json:"codeSnippet,omitempty"`
	StartLine      int     `json:"startLine,omitempty"`
	EndLine        int     `json:"endLine,omitempty"`
	Query          string  `json:"query,omitempty"`
	TopK           int     `json:"topK,omitempty"`
	ScoreThreshold float64 `json:"scoreThreshold,omitempty"`
	MaxLayer       int     `json:"maxLayer,omitempty"`
	IncludeContent bool    `json:"includeContent,omitempty"`
}

// ResponseData 响应数据结构
type ResponseData struct {
	Data struct {
		List []map[string]interface{} `json:"list"`
	} `json:"data"`
}

// doRequest 发送HTTP请求
func (c *APIClient) doRequest(ctx context.Context, url string, params interface{}, headers map[string]string, method string) (*ResponseData, error) {
	var req *http.Request
	var err error

	// 准备请求体
	var body []byte
	if method == "POST" {
		body, err = json.Marshal(params)
		if err != nil {
			zap.L().Error("Failed to marshal request params", zap.Error(err))
			return nil, err
		}
		req, err = http.NewRequestWithContext(ctx, method, url, bytes.NewBuffer(body))
	} else {
		req, err = http.NewRequestWithContext(ctx, method, url, nil)
	}

	if err != nil {
		zap.L().Error("Failed to create request", zap.Error(err), zap.String("url", url))
		return nil, err
	}

	// 设置请求头,只包含这几个
	// "x-request-id": headers.get("x-request-id", ""),
	// "authorization": headers.get("authorization", ""),
	// "x-costrict-version": headers.get("x-costrict-version", ""),
	if headers != nil {
		req.Header.Set("x-request-id", headers["x-request-id"])
		req.Header.Set("authorization", headers["authorization"])
		req.Header.Set("x-costrict-version", headers["x-costrict-version"])
	}
	req.Header.Set("Content-Type", "application/json")

	// 发送请求
	resp, err := c.client.Do(req)
	if err != nil {
		zap.L().Error("Request failed", zap.Error(err), zap.String("url", url))
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		zap.L().Warn("Request returned non-200 status",
			zap.Int("status", resp.StatusCode),
			zap.String("url", url))
		return nil, fmt.Errorf("request failed with status %d", resp.StatusCode)
	}

	// 解析响应
	var result ResponseData
	if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
		zap.L().Error("Failed to decode response", zap.Error(err))
		return nil, err
	}

	return &result, nil
}

// SearchDefinition 搜索代码定义
func (c *APIClient) SearchDefinition(ctx context.Context, clientID, codebasePath, filePath, codeSnippet string, startLine, endLine *int, headers map[string]string) (*ResponseData, error) {
	params := RequestParam{
		ClientID:     clientID,
		CodebasePath: codebasePath,
		FilePath:     filePath,
		CodeSnippet:  codeSnippet,
	}

	if startLine != nil {
		params.StartLine = *startLine
	}
	if endLine != nil {
		params.EndLine = *endLine
	}

	return c.doRequest(ctx, c.definitionURL, params, headers, "GET")
}

// SearchSemantic 语义搜索
func (c *APIClient) SearchSemantic(ctx context.Context, clientID, codebasePath, query string, headers map[string]string) (*ResponseData, error) {
	params := RequestParam{
		ClientID:       clientID,
		CodebasePath:   codebasePath,
		Query:          query,
		TopK:           config.SemanticTopK,
		ScoreThreshold: config.SemanticScoreThreshold,
	}

	return c.doRequest(ctx, c.semanticURL, params, headers, "POST")
}

// SearchRelation 关系检索
func (c *APIClient) SearchRelation(ctx context.Context, clientID, codebasePath, filePath, codeSnippet string, includeContent bool, maxLayer int, headers map[string]string) (*ResponseData, error) {
	params := RequestParam{
		ClientID:       clientID,
		CodebasePath:   codebasePath,
		FilePath:       filePath,
		CodeSnippet:    codeSnippet,
		MaxLayer:       maxLayer,
		IncludeContent: includeContent,
	}

	return c.doRequest(ctx, c.relationURL, params, headers, "GET")
}
