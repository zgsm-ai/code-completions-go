# Code Completions

Costrict code completions server for Golang. 


## Development

由于使用了tokenizer,只能在linux系统上开发，需要安装gcc.

### env prepare


```bash
sudo apt update
# install gcc
sudo apt install build-essential
# install git lfs
sudo apt install git-lfs
```


### go mod prepare

```bash
go mod tidy
```