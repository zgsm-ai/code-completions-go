package main

import (
	"flag"
	"os"

	_ "code-completions/pkg/logger"
	"code-completions/server"

	"github.com/gin-gonic/gin"
	"go.uber.org/zap"
)

func main() {
	// 解析命令行参数
	var (
		port = flag.String("port", "8080", "服务器端口")
		mode = flag.String("mode", "release", "运行模式 (debug/release)")
	)
	flag.Parse()

	// 设置Gin运行模式
	if *mode == "release" {
		gin.SetMode(gin.ReleaseMode)
	}

	// 创建zap日志实例
	logger, err := createLogger(*mode)
	if err != nil {
		panic("无法创建日志实例: " + err.Error())
	}
	defer logger.Sync()

	// 创建路由
	r := server.SetupRouter(logger)

	// 创建服务器
	addr := ":" + *port
	srv := server.NewServer(addr, r, logger)

	// 启动服务器
	if err := srv.Start(); err != nil {
		logger.Fatal("服务器运行失败", zap.Error(err))
		os.Exit(1)
	}
}

// createLogger 根据模式创建日志实例
func createLogger(mode string) (*zap.Logger, error) {
	if mode == "debug" {
		return zap.NewDevelopment()

	}
	return zap.NewProduction()
	
}
