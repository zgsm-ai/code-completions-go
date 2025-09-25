package completions

import (
	"code-completions/pkg/completions"

	"github.com/gin-gonic/gin"
)

// Completions 补全接口路由处理
func Completions(c *gin.Context) {
	completionHandler := completions.NewCompletionHandler()
	if completionHandler == nil {
		c.JSON(500, gin.H{"error": "failed to create completion handler"})
		return
	}
	completionHandler.GinHandler(c)
}
