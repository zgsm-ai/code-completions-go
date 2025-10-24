package model

import (
	"bytes"
	"code-completion/pkg/config"
	"code-completion/pkg/tokenizers"
	"context"
	"encoding/json"
	"io"
	"net/http"
	"time"

	"go.uber.org/zap"
)

type CompletionStatus string

const (
	CompletionSuccess     CompletionStatus = "success"     //补全成功
	CompletionModelError  CompletionStatus = "modelError"  //模型响应错误
	CompletionReqError    CompletionStatus = "reqError"    //请求存在错误
	CompletionServerError CompletionStatus = "serverError" //服务端错误
	CompletionEmpty       CompletionStatus = "empty"       //补全结果为空
	CompletionRejected    CompletionStatus = "rejected"    //根据规则拒绝补全
	CompletionTimeout     CompletionStatus = "timeout"     //补全请求超时
	CompletionCanceled    CompletionStatus = "canceled"    //用户取消
)

type OpenAIModel struct {
	Config    config.ModelConfig
	Tokenizer *tokenizers.Tokenizer
}

func (m *OpenAIModel) Completions(ctx context.Context, data map[string]interface{}, requestId string) (map[string]interface{}, CompletionStatus, *time.Time, *time.Time) {
	if data == nil {
		zap.L().Warn("req body is null", zap.String("requestId", requestId))
		return nil, CompletionReqError, nil, nil
	}

	// 修改data中的字段
	data["model"] = m.Config.ModelName
	data["max_tokens"] = m.Config.MaxOuputToken
	data["stream"] = false

	// 将data转换为JSON
	jsonData, err := json.Marshal(data)
	if err != nil {
		zap.L().Warn("json Marshal Error", zap.String("requestId", requestId), zap.Error(err))
		return nil, CompletionServerError, nil, nil
	}

	// 创建HTTP请求
	req, err := http.NewRequestWithContext(ctx, "POST", m.Config.CompletionsUrl, bytes.NewBuffer(jsonData))
	if err != nil {
		zap.L().Warn("create request error", zap.String("requestId", requestId), zap.Error(err))
		return nil, CompletionReqError, nil, nil
	}

	// 设置请求头
	req.Header.Set("Content-Type", "application/json")
	req.Header.Set("Authorization", m.Config.Authorization)

	// 发送请求
	client := &http.Client{
		Timeout: m.Config.Timeout,
	}
	modelStartTime := time.Now()
	resp, err := client.Do(req)
	modelEndTime := time.Now()
	if err != nil {
		zap.L().Warn("Request model error", zap.String("requestId", requestId),
			zap.Duration("latency", modelEndTime.Sub(modelStartTime)),
			zap.Error(err))
		return nil, CompletionServerError, &modelStartTime, &modelEndTime
	}
	defer resp.Body.Close()
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		zap.L().Warn("Read resp.body error", zap.String("requestId", requestId),
			zap.Duration("latency", modelEndTime.Sub(modelStartTime)), zap.Error(err))
		return nil, CompletionServerError, &modelStartTime, &modelEndTime
	}
	if resp.StatusCode < 200 || resp.StatusCode >= 300 {
		zap.L().Warn("Response Status error", zap.Int("status", resp.StatusCode),
			zap.String("requestId", requestId),
			zap.Duration("latency", modelEndTime.Sub(modelStartTime)),
			zap.String("respBody", string(body)))
		return nil, CompletionModelError, &modelStartTime, &modelEndTime
	}

	var result map[string]interface{}
	if err := json.Unmarshal(body, &result); err != nil {
		zap.L().Warn("json unmarshal error", zap.String("requestId", requestId),
			zap.Duration("latency", modelEndTime.Sub(modelStartTime)), zap.Error(err))
		return nil, CompletionServerError, &modelStartTime, &modelEndTime
	}
	zap.L().Info("Request model succeeded", zap.String("requestId", requestId),
		zap.Duration("latency", modelEndTime.Sub(modelStartTime)),
		zap.String("respBody", string(body)))
	return result, CompletionSuccess, &modelStartTime, &modelEndTime
}
