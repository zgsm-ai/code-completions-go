package tokenizers

import (
	"testing"

	"github.com/daulet/tokenizers"
)

// to test tokenizer
// sudo apt install build-essential
// export CGO_LDFLAGS="-L./build/library/libtokenizers"
// export CGO_ENABLED=1
// go test ./pkg/tokennizers/ -v
func Test_simple(t *testing.T) {
	tk, err := tokenizers.FromFile("../../bin/cgtok/starcoder_tokenizer.json")
	if err != nil {
		t.Error(err)
		return
	}
	// release native resources
	defer tk.Close()

	t.Log("Vocab size:", tk.VocabSize())
	// Vocab size: 30522
	t.Log(tk.Encode("brown fox jumps over the lazy dog", false))
	// [2829 4419 14523 2058 1996 13971 3899] [brown fox jumps over the lazy dog]
	t.Log(tk.Encode("brown fox jumps over the lazy dog", true))
	// [101 2829 4419 14523 2058 1996 13971 3899 102] [[CLS] brown fox jumps over the lazy dog [SEP]]
	t.Log(tk.Decode([]uint32{2829, 4419, 14523, 2058, 1996, 13971, 3899}, true))

	ei, es := tk.Encode("hello world!", false)
	t.Log(ei, es)
	di := tk.Decode(ei, true)
	t.Log(di)
}
