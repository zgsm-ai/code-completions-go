package completions

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/model"
	"code-completion/pkg/stream_controller"
	"net/http"
	"time"

	"github.com/gin-gonic/gin"
)

// Completions 补全接口路由处理
// @Summary 代码补全
// @Description 根据提供的代码上下文生成代码补全建议
// @Tags completions
// @Accept json
// @Produce json
// @Param request body completions.CompletionRequest true "补全请求"
// @Success 200 {object} completions.CompletionResponse
// @Failure 400 {object} map[string]interface{}
// @Failure 500 {object} map[string]interface{}
// @Router /api/completions [post]
// @Router /code-completion/api/v1/completions [post]
func Completions(c *gin.Context) {
	var req completions.CompletionRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	// 使用流控管理器处理请求
	if stream_controller.Controller != nil {
		response := stream_controller.Controller.ProcessCompletionRequest(c.Request.Context(), &req, c.Request.Header)
		// if err != nil {
		// 	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		// 	return
		// }
		// c.JSON(http.StatusOK, response)
		respCompletion(c, response)
		return
	}

	// 如果流控管理器未初始化，使用原有逻辑
	handler := completions.NewCompletionHandler()
	perf := &completions.CompletionPerformance{
		ReceiveTime: time.Now(),
	}
	rc := completions.NewCompletionContext(c.Request.Context(), perf)
	response := handler.HandleCompletion(rc, &req, c.Request.Header)
	// if err != nil {
	// 	c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
	// 	return
	// }
	// 返回响应
	//c.JSON(http.StatusOK, response)
	respCompletion(c, response)
}

func respCompletion(c *gin.Context, rsp *completions.CompletionResponse) {
	if rsp.Status != model.CompletionSuccess {
		//c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		c.JSON(http.StatusInternalServerError, rsp)
	} else {
		c.JSON(http.StatusOK, rsp)
	}
}
