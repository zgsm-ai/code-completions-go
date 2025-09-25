package codebase_context

import (
	"go.uber.org/zap"
)

// Init 初始化上下文模块
func Init() error {
	zap.L().Info("Initializing codebase context module")

	// 确保配置已经初始化
	if config == nil {
		zap.L().Error("Configuration not initialized")
		return nil
	}

	zap.L().Info("Codebase context module initialized successfully",
		zap.String("definition_url", config.CodebaseDefinitionURL),
		zap.String("semantic_url", config.CodebaseSemanticURL),
		zap.String("relation_url", config.CodebaseRelationURL),
		zap.Bool("definition_search_enabled", config.EnableDefinitionSearch),
		zap.Bool("semantic_search_enabled", config.EnableSemanticSearch),
		zap.Duration("request_timeout", config.RequestTimeout),
		zap.Duration("context_cost_time", config.ContextCostTime),
	)

	return nil
}

// GetContextClient 获取上下文客户端实例
func GetContextClient() *ContextClient {
	return NewContextClient()
}

// GetAPIClient 获取API客户端实例
func GetAPIClient() *APIClient {
	return NewAPIClient()
}
