package tokenizers

import (
	"strings"

	"github.com/pkoukk/tiktoken-go"
)

// Tokenizer 包装tiktoken-go库，提供统一的接口
type Tokenizer struct {
	encoding *tiktoken.Tiktoken
}

// NewTokenizer 创建新的tokenizer实例
func NewTokenizer(tokenizerPath string) (*Tokenizer, error) {
	// 对于StarCoder模型，使用p50k_base编码
	// 注意：tiktoken-go不直接从文件加载tokenizer，而是使用预定义的编码
	// 如果需要自定义tokenizer，可能需要使用其他方法
	encoding, err := tiktoken.GetEncoding("p50k_base")
	if err != nil {
		// 如果获取预定义编码失败，尝试使用cl100k_base（GPT-4的编码）
		encoding, err = tiktoken.GetEncoding("cl100k_base")
		if err != nil {
			// 如果还是失败，使用模型名称获取编码
			encoding, err = tiktoken.EncodingForModel("gpt-4")
			if err != nil {
				return nil, err
			}
		}
	}

	return &Tokenizer{encoding: encoding}, nil
}

// Encode 将文本编码为token IDs
func (t *Tokenizer) Encode(text string) []int {
	// tiktoken-go的Encode方法需要三个参数：text, allowedSpecial, disallowedSpecial
	// 对于大多数情况，我们可以使用空字符串数组
	allowedSpecial := []string{}
	disallowedSpecial := []string{}

	return t.encoding.Encode(text, allowedSpecial, disallowedSpecial)
}

// Decode 将token IDs解码为文本
func (t *Tokenizer) Decode(ids []int) string {
	// 直接使用tiktoken-go的Decode方法
	return t.encoding.Decode(ids)
}

// GetTokenCount 获取文本的token数量
func (t *Tokenizer) GetTokenCount(text string) int {
	// 使用tiktoken-go的Encode方法获取token数量
	allowedSpecial := []string{}
	disallowedSpecial := []string{}
	ids := t.encoding.Encode(text, allowedSpecial, disallowedSpecial)
	return len(ids)
}

// GetTokens 获取文本的token列表
func (t *Tokenizer) GetTokens(text string) []int {
	return t.Encode(text)
}

// ConvertNLToLinux 将Windows换行符转换为Linux换行符
func ConvertNLToLinux(s string) string {
	return strings.ReplaceAll(s, "\r\n", "\n")
}

// ConvertNLToWin 将Linux换行符转换为Windows换行符
func ConvertNLToWin(s string) string {
	return strings.ReplaceAll(s, "\n", "\r\n")
}

// Close 释放资源
func (t *Tokenizer) Close() {
	// tiktoken-go不需要显式释放资源
}
