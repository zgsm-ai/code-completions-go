package completions

import (
	"code-completion/pkg/completions"
	"code-completion/pkg/model"
	"code-completion/pkg/stream_controller"
	"net/http"
	"time"

	"github.com/gin-gonic/gin"
	"go.uber.org/zap"
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
		rsp := stream_controller.Controller.ProcessCompletionRequest(c.Request.Context(), &req, c.Request.Header)
		respCompletion(c, &req, rsp)
		return
	}

	// 如果流控管理器未初始化，使用原有逻辑
	handler := completions.NewCompletionHandler(nil)
	perf := &completions.CompletionPerformance{
		ReceiveTime: time.Now().Local(),
	}
	rc := completions.NewCompletionContext(c.Request.Context(), perf)
	rsp := handler.HandleCompletion(rc, &req, c.Request.Header)
	respCompletion(c, &req, rsp)
}

func respCompletion(c *gin.Context, req *completions.CompletionRequest, rsp *completions.CompletionResponse) {
	if rsp.Status != model.CompletionSuccess {
		zap.L().Warn("completion error", zap.String("completionID", rsp.ID),
			zap.String("clientID", req.ClientID),
			zap.String("status", string(rsp.Status)),
			zap.Any("response", rsp))
	} else {
		zap.L().Info("completion succeeded", zap.String("completionID", rsp.ID),
			zap.String("clientID", req.ClientID),
			zap.Any("response", rsp))
	}
	statusCode := http.StatusOK
	switch rsp.Status {
	case model.CompletionSuccess:
		statusCode = http.StatusOK
	case model.CompletionCanceled:
		statusCode = http.StatusRequestTimeout
	case model.CompletionTimeout:
		statusCode = http.StatusGatewayTimeout
	case model.CompletionReqError:
	case model.CompletionRejected:
		statusCode = http.StatusBadRequest
	default:
		statusCode = http.StatusInternalServerError
	}
	c.JSON(statusCode, rsp)
}
