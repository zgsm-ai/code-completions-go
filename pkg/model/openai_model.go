package model

import (
	"bytes"
	"code-completion/pkg/config"
	"code-completion/pkg/tokenizers"
	"context"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"strings"
)

type OpenAIModel struct {
	cfg       *config.ModelConfig
	tokenizer *tokenizers.Tokenizer
}

func NewOpenAIModel(c *config.ModelConfig, t *tokenizers.Tokenizer) LLM {
	return &OpenAIModel{
		cfg:       c,
		tokenizer: t,
	}
}

func (m *OpenAIModel) Config() *config.ModelConfig {
	return m.cfg
}

func (m *OpenAIModel) Tokenizer() *tokenizers.Tokenizer {
	return m.tokenizer
}

/**
 * 获取加了FIM标记的prompt文本
 */
func (m *OpenAIModel) getFimPrompt(prefix, suffix, codeContext string, cfg *config.ModelConfig) string {
	return cfg.FimBegin + codeContext + "\n" + prefix + cfg.FimHole + suffix + cfg.FimEnd
}

func (m *OpenAIModel) Completions(ctx context.Context, p *CompletionParameter) (*CompletionResponse, *CompletionVerbose, CompletionStatus, error) {
	var prefix string
	if m.cfg.FimMode {
		prefix = m.getFimPrompt(p.Prefix, p.Suffix, p.CodeContext, m.cfg)
	} else {
		if p.CodeContext != "" {
			prefix = strings.Join([]string{p.CodeContext, p.Prefix}, "\n")
		} else {
			prefix = p.Prefix
		}
	}
	maxTokens := min(p.MaxTokens, m.cfg.MaxOuputToken)
	data := map[string]interface{}{
		"model":       m.cfg.ModelName,
		"prompt":      prefix,
		"stop":        p.Stop,
		"temperature": p.Temperature,
		"max_tokens":  maxTokens,
		"stream":      false,
		// "presence_penalty":  0.0,
		// "frequency_penalty": 0.0,
		// "top_p":             1,
		// "suffix": p.Suffix,
	}
	if !m.cfg.FimMode && p.Suffix != "" {
		data["suffix"] = p.Suffix
	}
	var verbose CompletionVerbose
	verbose.Provider = m.cfg.Provider
	verbose.Input = data
	// 将data转换为JSON
	jsonData, err := json.Marshal(data)
	if err != nil {
		return nil, &verbose, CompletionServerError, err
	}

	// 创建HTTP请求
	req, err := http.NewRequestWithContext(ctx, "POST", m.cfg.CompletionsUrl, bytes.NewBuffer(jsonData))
	if err != nil {
		return nil, &verbose, CompletionReqError, err
	}

	// 设置请求头
	req.Header.Set("Content-Type", "application/json")
	req.Header.Set("Authorization", m.cfg.Authorization)

	// 发送请求
	client := &http.Client{
		Timeout: m.cfg.Timeout,
	}
	resp, err := client.Do(req)
	if err != nil {
		status := CompletionServerError
		if err == context.Canceled {
			status = CompletionCanceled
		} else if err == context.DeadlineExceeded {
			status = CompletionTimeout
		}
		return nil, &verbose, status, err
	}
	defer resp.Body.Close()
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, &verbose, CompletionServerError, err
	}
	json.Unmarshal(body, &verbose.Output)
	if resp.StatusCode < 200 || resp.StatusCode >= 300 {
		return nil, &verbose, CompletionModelError, fmt.Errorf("Invalid StatusCode(%d)", resp.StatusCode)
	}
	var rsp CompletionResponse
	if err := json.Unmarshal(body, &rsp); err != nil {
		return nil, &verbose, CompletionServerError, err
	}
	return &rsp, &verbose, CompletionSuccess, nil
}

func (m *OpenAIModel) getCompletionCode(result map[string]interface{}) (string, error) {
	var completionText string
	// 从模型结果中获取补全文本，这里需要根据实际的模型返回结构进行调整
	if result != nil {
		if choices, ok := result["choices"].([]interface{}); ok && len(choices) > 0 {
			// 获取第一个choice作为主要补全文本
			if choice, ok := choices[0].(map[string]interface{}); ok {
				if text, ok := choice["text"].(string); ok {
					completionText = text
				}
			}
		}
	}
	return completionText, nil
}
