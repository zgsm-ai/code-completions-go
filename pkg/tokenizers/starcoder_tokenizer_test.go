package tokenizers

import (
	"testing"
)

// to test tokenizer
// go test ./pkg/tokenizers/ -v
func Test_simple(t *testing.T) {
	tk, err := NewTokenizer("../../bin/cgtok/starcoder_tokenizer.json")
	if err != nil {
		t.Error(err)
		return
	}
	// release native resources
	defer tk.Close()

	// Note: tiktoken-go doesn't provide VocabSize method directly
	// t.Log("Vocab size:", tk.VocabSize())

	// Test encoding
	tokens := tk.Encode("brown fox jumps over the lazy dog")
	t.Log("Encoded tokens:", tokens)

	// Test decoding
	decoded := tk.Decode([]int{2829, 4419, 14523, 2058, 1996, 13971, 3899})
	t.Log("Decoded text:", decoded)

	// Test with hello world
	ei := tk.Encode("hello world!")
	t.Log("Encoded 'hello world!':", ei)
	di := tk.Decode(ei)
	t.Log("Decoded back:", di)

	// Test token count
	count := tk.GetTokenCount("hello world!")
	t.Log("Token count for 'hello world!':", count)
}
