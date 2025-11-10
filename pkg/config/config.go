package config

import (
	"encoding/json"
	"fmt"
	"os"
	"strings"
	"time"

	"gopkg.in/yaml.v3"
)

type ModelConfig struct {
	Provider         string        `json:"provider" yaml:"provider"`                 // 模型供应商，代表着具体的模型接口/类型
	ModelId          string        `json:"modelId" yaml:"modelId"`                   // 模型来源的唯一标识
	ModelName        string        `json:"modelName" yaml:"modelName"`               // 真实的模型名称
	CompletionsUrl   string        `json:"completionsUrl" yaml:"completionsUrl"`     // 补全地址
	Tags             []string      `json:"tags" yaml:"tags"`                         // 模型标签，用户可以根据标签选择补全模型
	Authorization    string        `json:"authorization" yaml:"authorization"`       // 认证信息
	Timeout          time.Duration `json:"timeout" yaml:"timeout"`                   // 超时时间ms
	MaxPrefixContext int           `json:"maxPrefixContext" yaml:"maxPrefixContext"` // 最大模型上下文长度:前缀
	MaxSuffixContext int           `json:"maxSuffixContext" yaml:"maxSuffixContext"` // 最大模型上下文长度:后缀
	MaxOutputToken   int           `json:"maxOutputToken" yaml:"maxOutputToken"`     // 最大输出token数
	FimMode          bool          `json:"fimMode" yaml:"fimMode"`                   // 填充FIM标记的模式
	FimBegin         string        `json:"fimBegin" yaml:"fimBegin"`                 // 开始
	FimEnd           string        `json:"fimEnd" yaml:"fimEnd"`                     // 结束
	FimHole          string        `json:"fimHole" yaml:"fimHole"`                   // 待补全的空洞位置
	FimStop          []string      `json:"fimStop" yaml:"fimStop"`                   // 结束符
	TokenizerPath    string        `json:"tokenizerPath" yaml:"tokenizerPath"`       // tokenizer json 路径
	MaxConcurrent    int           `json:"maxConcurrent" yaml:"maxConcurrent"`       // 每种模型的最大并发数，防止模型过载
	DisablePrune     bool          `json:"disablePrune" yaml:"disablePrune"`         // 禁止后期修剪
	CustomPruners    []string      `json:"customPruners" yaml:"customPruners"`       // 自定义的后期修剪工具
}

type CodebaseContextConfig struct {
	DisableDefinitionSearch bool          `json:"disableDefinitionSearch" yaml:"disableDefinitionSearch"`
	DisableSemanticSearch   bool          `json:"disableSemanticSearch" yaml:"disableSemanticSearch"`
	DisableRelationSearch   bool          `json:"disableRelationSearch" yaml:"disableRelationSearch"`
	CodebaseDefinitionURL   string        `json:"codebaseDefinitionURL" yaml:"codebaseDefinitionURL"`
	CodebaseSemanticURL     string        `json:"codebaseSemanticURL" yaml:"codebaseSemanticURL"`
	CodebaseRelationURL     string        `json:"codebaseRelationURL" yaml:"codebaseRelationURL"`
	SemanticTopK            int           `json:"semanticTopK" yaml:"semanticTopK"`
	SemanticScoreThreshold  float64       `json:"semanticScoreThreshold" yaml:"semanticScoreThreshold"`
	RelationLayer           int           `json:"relationLayer" yaml:"relationLayer"`
	RelationIncludeContent  bool          `json:"relationIncludeContent" yaml:"relationIncludeContent"`
	RequestTimeout          time.Duration `json:"requestTimeout" yaml:"requestTimeout"` // 请求超时
	TotalTimeout            time.Duration `json:"totalTimeout" yaml:"totalTimeout"`     // 上下文总耗时上限
}

type StreamControllerConfig struct {
	MaintainInterval  time.Duration `json:"maintainInterval" yaml:"maintainInterval"`   // 定时维护的间隔
	CompletionTimeout time.Duration `json:"completionTimeout" yaml:"completionTimeout"` // 一个补全请求的最大超时
	QueueTimeout      time.Duration `json:"queueTimeout" yaml:"queueTimeout"`           // 排队超时
}

type CompletionWrapperConfig struct {
	DisableScore           bool    `json:"disableScore" yaml:"disableScore"`                     // 禁用隐藏分过滤器
	DisableLanguageFeature bool    `json:"disableLanguageFeature" yaml:"disableLanguageFeature"` // 禁用语言特性过滤器
	ThresholdScore         float64 `json:"thresholdScore" yaml:"thresholdScore"`                 // 阈值分数
	StrPattern             string  `json:"strPattern" yaml:"strPattern"`                         // 字符串模式
	TreePattern            string  `json:"treePattern" yaml:"treePattern"`                       // 树模式
	LineCountThreshold     int     `json:"lineCountThreshold" yaml:"lineCountThreshold"`         // 行数阈值
	EndTag                 string  `json:"endTag" yaml:"endTag"`                                 // 结束标签
}

type Conf struct {
	CodebaseContext   CodebaseContextConfig   `json:"codebaseContext" yaml:"codebaseContext"`     // 上下文配置
	Models            []ModelConfig           `json:"models" yaml:"models"`                       // 模型配置
	StreamController  StreamControllerConfig  `json:"streamController" yaml:"streamController"`   // 全局流控配置
	CompletionsConfig CompletionWrapperConfig `json:"completionsConfig" yaml:"completionsConfig"` // 补全的前后置处理
}

var Config = &Conf{}

func resetDefValues(c *Conf) {
	if c.StreamController.QueueTimeout == 0 {
		c.StreamController.QueueTimeout = 1000 * time.Millisecond
	}
	if c.StreamController.CompletionTimeout == 0 {
		c.StreamController.CompletionTimeout = 4500 * time.Microsecond
	}
}

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
	}
	resetDefValues(Config)
	data, _ := json.MarshalIndent(Config, "", "  ")
	fmt.Printf("配置文件加载成功:\n%s\n", string(data))
}
