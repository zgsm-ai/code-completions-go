package server

import (
	"net/http"
	"time"

	"code-completions/pkg/metrics"
	"code-completions/server/completions"

	"github.com/gin-gonic/gin"
	"go.uber.org/zap"
)

// SetupRouter 设置路由
func SetupRouter(logger *zap.Logger) *gin.Engine {
	// 创建Gin实例
	r := gin.New()

	// 使用自定义日志中间件
	r.Use(ginLogger(logger))

	// 使用恢复中间件，防止panic导致服务器崩溃
	r.Use(gin.Recovery())

	// 健康检查接口
	r.GET("/health", healthCheck)

	// Prometheus指标接口
	r.GET("/metrics", func(c *gin.Context) {
		metrics.GetMetricsHandler().ServeHTTP(c.Writer, c.Request)
	})

	// API路由组
	api := r.Group("/api")
	api.Use(func(c *gin.Context) {
		c.Writer.Header().Set("Content-Type", "application/json; charset=utf-8")
		c.Next()
	})
	{
		api.GET("/example", exampleHandler)
		api.POST("/completions", completions.Completions)
		api.Use(func(c *gin.Context) {
			c.Writer.Header().Set("Content-Type", "application/json; charset=utf-8")
			c.Next()
		})
	}
	completionRouter := r.Group("/code-completion")
	completionRouter.Use(func(c *gin.Context) {
		c.Writer.Header().Set("Content-Type", "application/json; charset=utf-8")
		c.Next()
	})
	{
		completionRouter.POST("/api/v1/completions", completions.Completions)

	}

	return r
}

// healthCheck 健康检查处理器
func healthCheck(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{
		"status": "ok",
		"time":   time.Now().Format(time.RFC3339),
	})
}

// exampleHandler 示例接口处理器
func exampleHandler(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{
		"message": "Hello, World!",
		"data": gin.H{
			"timestamp": time.Now().Unix(),
		},
	})
}

// ginLogger gin日志中间件
func ginLogger(logger *zap.Logger) gin.HandlerFunc {
	return func(c *gin.Context) {
		start := time.Now()
		path := c.Request.URL.Path

		// 处理请求
		c.Next()

		// 记录日志
		latency := time.Since(start)
		clientIP := c.ClientIP()
		method := c.Request.Method
		statusCode := c.Writer.Status()
		bodySize := c.Writer.Size()

		//if raw != "" {
		//	path = path + "?" + raw
		//}

		logger.Info("HTTP请求",
			zap.Int("status", statusCode),
			zap.String("method", method),
			zap.String("path", path),
			zap.String("ip", clientIP),
			zap.Duration("latency", latency),
			zap.Int("bodySize", bodySize),
		)
	}
}
