package model

import (
	"code-completions/pkg/config"
	"code-completions/pkg/tokenizers"
	"sync"

	"go.uber.org/zap"
)

type OpenAIModelManager struct {
	models []OpenAIModel
	mutex  sync.Mutex
	index  int
}

func (m *OpenAIModelManager) GetModel() *OpenAIModel {
	m.mutex.Lock()
	defer m.mutex.Unlock()
	modelLen := len(m.models)
	if modelLen == 0 {
		return nil
	}
	// 返回新的model,而不是原始的model。
	var model OpenAIModel
	if m.index < modelLen {
		model = m.models[m.index]
		m.index++
	} else {
		m.index = 1
		model = m.models[0]
	}
	return &model
}

var GlobalModelManager = &OpenAIModelManager{}

func init() {
	modelConfigs := config.Config.Models
	GlobalModelManager.models = make([]OpenAIModel, 0)
	for _, c := range modelConfigs {
		token, err := tokenizers.NewTokenizer(c.TokenizerPath)
		if err != nil {
			zap.L().Error("init tokenizer error", zap.String("tokenizerPath", c.TokenizerPath), zap.Error(err))
			continue
		}
		GlobalModelManager.models = append(GlobalModelManager.models,
			OpenAIModel{
				Config: ModelConfig{
					ModelName:      c.ModelName,
					CompletionsUrl: c.CompletionsUrl,
					Authorization:  c.Authorization,
					MaxOuputToken:  c.MaxOuputToken,
					FimPrefix:      c.FimPrefix,
					FimMiddle:      c.FimMiddle,
					FimStop:        c.FimStop,
					Timeout:        c.Timeout,
				},
				Tokenizer: token,
			},
		)
	}
}
