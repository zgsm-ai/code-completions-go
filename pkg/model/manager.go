package model

import (
	"code-completion/pkg/config"
	"code-completion/pkg/tokenizers"
	"fmt"
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
		panic(m)
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

func Init(cfgModels []config.ModelConfig) error {
	models := make([]OpenAIModel, 0)
	for _, c := range cfgModels {
		token, err := tokenizers.NewTokenizer(c.TokenizerPath)
		if err != nil {
			zap.L().Error("init tokenizer error", zap.String("tokenizerPath", c.TokenizerPath), zap.Error(err))
			continue
		}
		models = append(models, OpenAIModel{
			Config:    c,
			Tokenizer: token,
		})
	}
	if len(models) == 0 {
		zap.L().Fatal("No models available")
		return fmt.Errorf("no models available")
	}
	GlobalModelManager.models = models
	return nil
}
