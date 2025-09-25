package config

import (
	"fmt"
	"os"
	"strings"
	"time"

	"gopkg.in/yaml.v3"
)

type Conf struct {
	CodebaseContext struct {
		// 上下文检索url
		CodebaseDefinitionURL string
		CodebaseSemanticURL   string
		CodebaseRelationURL   string
		// 语义检索参数
		SemanticTopK           int
		SemanticScoreThreshold float64
		// 请求超时
		RequestTimeout time.Duration
		// 禁用项目
		DisEnableDefinitionSearch bool
		DisEnableSemanticSearch   bool
		// 耗时
		ContextCostTime time.Duration
	}
	Models []struct {
		CompletionsUrl string   `yaml:"completionsUrl"` // 补全地址
		ModelName      string   `yaml:"modelName"`      // 模型名称
		Authorization  string   `yaml:"authorization"`  // 认证信息
		MaxTokens      int      `yaml:"maxContext"`     // 最大模型上下文长度
		MaxOuputToken  int      `yaml:"maxOutputToken"` // 最大输出token数
		FimPrefix      string   `yaml:"fimPrefix"`      // 前缀
		FimMiddle      string   `yaml:"fimMiddle"`      // 中缀
		FimSuffix      string   `yaml:"fimSuffix"`      // 后缀
		FimStop        []string `yaml:"fimStop"`        // 结束符
		TokenizerPath  string   `yaml:"tokenizerPath"`  // tokenizer json 路径
		Timeout        int      `yaml:"timeout"`        // 超时时间
	} `yaml:"models"`
	CompletionsConfig struct {
		DisalbeScoreReject           bool    `yaml:"disableScore"`           // 禁用隐藏分拒绝器
		DisableLanguageFeatureReject bool    `yaml:"disableLanguageFeature"` // 禁用语言特性拒绝器
		ThresholdScore               float64 `yaml:"thresholdScore"`         // 阈值分数
		StrPattern                   string  `yaml:"strPattern"`             // 字符串模式
		TreePattern                  string  `yaml:"treePattern"`            // 树模式
		LineCountThreshold           int     `yaml:"lineCountThreshold"`     // 行数阈值
		EndTag                       string  `yaml:"endTag"`                 // 结束标签
	} `yaml:"completionsConfig"`
}

var Config = &Conf{}

func init() {
	// 读取配置文件
	configFile, err := os.ReadFile("config.yaml")
	if err != nil {
		fmt.Printf("读取配置文件失败: %v\n", err)
		return
	}

	configFileStr := strings.ReplaceAll(string(configFile), "\r\n", "\n")

	// 解析 YAML 配置
	err = yaml.Unmarshal([]byte(configFileStr), Config)
	if err != nil {
		fmt.Printf("解析配置文件失败: %v\n", err)
		panic(err)
		return
	}

	fmt.Println("配置文件加载成功")
}
