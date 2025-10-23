package codebase_context

import (
	"code-completion/pkg/config"
)

var contextConfig *config.CodebaseContextConfig

func init() {
	contextConfig = &config.Config.CodebaseContext
}
