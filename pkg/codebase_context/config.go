package codebase_context

import (
	globalConfig "code-completions/pkg/config"
	"time"
)

type ContextReqConfig struct {
	// API URLs
	CodebaseDefinitionURL string
	CodebaseSemanticURL   string
	CodebaseRelationURL   string

	// Semantic search parameters
	SemanticTopK           int
	SemanticScoreThreshold float64

	// Request timeout
	RequestTimeout time.Duration

	// Feature flags
	EnableDefinitionSearch bool
	EnableSemanticSearch   bool

	// Context cost time
	ContextCostTime time.Duration
}

var config *ContextReqConfig

func init() {
	config = &ContextReqConfig{
		CodebaseDefinitionURL:  globalConfig.Config.CodebaseContext.CodebaseDefinitionURL,
		CodebaseSemanticURL:    globalConfig.Config.CodebaseContext.CodebaseSemanticURL,
		CodebaseRelationURL:    globalConfig.Config.CodebaseContext.CodebaseRelationURL,
		SemanticTopK:           globalConfig.Config.CodebaseContext.SemanticTopK,
		SemanticScoreThreshold: globalConfig.Config.CodebaseContext.SemanticScoreThreshold,
		RequestTimeout:         globalConfig.Config.CodebaseContext.RequestTimeout,
		EnableDefinitionSearch: !globalConfig.Config.CodebaseContext.DisEnableDefinitionSearch,
		EnableSemanticSearch:   !globalConfig.Config.CodebaseContext.DisEnableSemanticSearch,
		ContextCostTime:        globalConfig.Config.CodebaseContext.ContextCostTime,
	}
	_ = Init()
}
