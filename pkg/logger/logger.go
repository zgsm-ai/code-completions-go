package logger

import (
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
)

// 全局 logger 实例
var Logger *zap.Logger

func init() {
	// 使用 NewProductionConfig 并可选调整日志级别
	config := zap.NewProductionConfig()
	config.Level = zap.NewAtomicLevelAt(zapcore.InfoLevel) // 设置日志级别为 Info

	// 注意：禁止将日志输出到文件
	// config.OutputPaths = []string{"./app.log"}

	var err error
	Logger, err = config.Build()
	if err != nil {
		panic(err)
	}

	// 替换 zap 的全局 logger
	zap.ReplaceGlobals(Logger)
}

// InitLogger 初始化全局 logger（已废弃，logger 在包 init 时自动初始化）
func InitLogger() {
	// logger 已经在 init 函数中自动初始化，此函数保留仅为兼容性
}

// Sync 刷新所有日志到输出
func Sync() {
	Logger.Sync()
}

// 便捷函数，直接调用全局 logger 的方法

// Info 记录信息级别日志
func Info(msg string, fields ...zap.Field) {
	Logger.Info(msg, fields...)
}

// Error 记录错误级别日志
func Error(msg string, fields ...zap.Field) {
	Logger.Error(msg, fields...)
}

// Debug 记录调试级别日志
func Debug(msg string, fields ...zap.Field) {
	Logger.Debug(msg, fields...)
}

// Warn 记录警告级别日志
func Warn(msg string, fields ...zap.Field) {
	Logger.Warn(msg, fields...)
}

// Fatal 记录致命错误级别日志
func Fatal(msg string, fields ...zap.Field) {
	Logger.Fatal(msg, fields...)
}

// Panic 记录恐慌级别日志
func Panic(msg string, fields ...zap.Field) {
	Logger.Panic(msg, fields...)
}

// With 创建带有额外字段的 logger
func With(fields ...zap.Field) *zap.Logger {
	return Logger.With(fields...)
}

// ExampleUsage 示例使用函数
