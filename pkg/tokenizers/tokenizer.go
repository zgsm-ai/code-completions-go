package tokenizers

import (
	"strings"

	"github.com/daulet/tokenizers"
)

// Tokenizer 包装tokenizers库，提供统一的接口
type Tokenizer struct {
	tokenizer *tokenizers.Tokenizer
}

// NewTokenizer 创建新的tokenizer实例
func NewTokenizer(tokenizerPath string) (*Tokenizer, error) {
	tk, err := tokenizers.FromFile(tokenizerPath)
	if err != nil {
		return nil, err
	}
	return &Tokenizer{tokenizer: tk}, nil
}

// Encode 将文本编码为token IDs
func (t *Tokenizer) Encode(text string, addSpecialTokens bool) []uint32 {
	ids, _ := t.tokenizer.Encode(text, addSpecialTokens)
	return ids
}

// Decode 将token IDs解码为文本
func (t *Tokenizer) Decode(ids []uint32, skipSpecialTokens bool) string {
	return t.tokenizer.Decode(ids, skipSpecialTokens)
}

// GetTokenCount 获取文本的token数量
func (t *Tokenizer) GetTokenCount(text string) int {
	return len(t.Encode(text, false))
}

// GetTokens 获取文本的token列表
func (t *Tokenizer) GetTokens(text string) []uint32 {
	return t.Encode(text, false)
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
	if t.tokenizer != nil {
		t.tokenizer.Close()
	}
}
