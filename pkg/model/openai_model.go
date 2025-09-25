package model

import (
	"bytes"
	"code-completions/pkg/tokenizers"
	"context"
	"encoding/json"
	"io"
	"net/http"
	"time"

	"go.uber.org/zap"
)

type ModelConfig struct {
	CompletionsUrl   string   `json:"completionsUl" yaml:"completionsUrl"`      // 补全地址
	ModelName        string   `json:"modelName" yaml:"modelName"`               // 模型名称
	Authorization    string   `json:"authorization" yaml:"authorization"`       // 认证信息
	Timeout          int      `json:"timeout" yaml:"timeout"`                   // 超时时间ms
	MaxPrefixContext int      `json:"maxPrefixContext" yaml:"maxPrefixContext"` // 最大模型上下文长度:前缀
	MaxSuffixContext int      `json:"maxSuffixContext" yaml:"maxSuffixContext"` // 最大模型上下文长度:后缀
	MaxOuputToken    int      `json:"maxOutputToken" yaml:"maxOutputToken"`     // 最大输出token数
	FimPrefix        string   `json:"fimPrefix" yaml:"fimPrefix"`               // 前缀
	FimMiddle        string   `json:"fimMiddle" yaml:"fimMiddle"`               // 中缀
	FimSuffix        string   `json:"fimSuffix" yaml:"fimSuffix"`               // 后缀
	FimStop          []string `json:"fimStop" yaml:"fimStop"`                   // 结束符
}

type CompletionStatus string

const (
	CompletionSucess       CompletionStatus = "sucess"
	CompletionModel        CompletionStatus = "modelError"
	CompletionReqError     CompletionStatus = "reqError"
	CompletionEmpty        CompletionStatus = "empty"
	CompletionProcessEmpty CompletionStatus = "processEmpty"
	CompletionServerError  CompletionStatus = "serverError"
	CompletionReject       CompletionStatus = "reject"
	CompletionTimeout      CompletionStatus = "timeout"
)

type OpenAIModel struct {
	Config    ModelConfig
	Tokenizer *tokenizers.Tokenizer
}

func (m *OpenAIModel) Completions(ctx context.Context, data map[string]interface{}, requestId string) (map[string]interface{}, CompletionStatus, *time.Time, *time.Time) {

	if data == nil {
		zap.L().Error("req body is null", zap.String("requestId", requestId))
		return nil, CompletionReqError, nil, nil
	}

	// 修改data中的字段
	data["model"] = m.Config.ModelName
	data["max_tokens"] = m.Config.MaxOuputToken
	data["stream"] = false

	// 将data转换为JSON
	jsonData, err := json.Marshal(data)
	if err != nil {
		zap.L().Error("json Marshal Error", zap.String("requestId", requestId), zap.String("error", err.Error()))
		return nil, CompletionServerError, nil, nil
	}

	// 创建HTTP请求
	req, err := http.NewRequestWithContext(ctx, "POST", m.Config.CompletionsUrl, bytes.NewBuffer(jsonData))
	if err != nil {
		zap.L().Error("create request error", zap.String("requestId", requestId), zap.String("error", err.Error()))
		return nil, CompletionReqError, nil, nil
	}

	// 设置请求头
	req.Header.Set("Content-Type", "application/json")
	req.Header.Set("Authorization", m.Config.Authorization)

	// 发送请求
	client := &http.Client{
		Timeout: time.Duration(m.Config.Timeout) * time.Millisecond,
	}
	modelStartTime := time.Now()
	resp, err := client.Do(req)
	modelEndTime := time.Now()
	if err != nil {
		zap.L().Error("Do Request Error", zap.String("requestId", requestId), zap.String("error", err.Error()))
		return nil, CompletionReqError, &modelStartTime, &modelEndTime
	}
	defer resp.Body.Close()

	// 检查HTTP状态码
	if resp.StatusCode != http.StatusOK {
		zap.L().Error("resp Status error", zap.Int("http code", resp.StatusCode), zap.String("requestId", requestId))
		return nil, CompletionReqError, &modelStartTime, &modelEndTime
	}

	// 读取响应体
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		zap.L().Error("resp Status error", zap.String("requestId", requestId), zap.String("error", err.Error()))
		return nil, CompletionReqError, &modelStartTime, &modelEndTime
	}

	// 解析JSON响应
	var result map[string]interface{}
	if err := json.Unmarshal(body, &result); err != nil {
		zap.L().Error("json unmarshal error", zap.String("requestId", requestId), zap.String("error", err.Error()))
		return nil, CompletionServerError, &modelStartTime, &modelEndTime
	}

	return result, CompletionSucess, &modelStartTime, &modelEndTime
}
