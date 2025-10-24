#!/bin/sh

ADDR=""
DATA=""
PROMPT=""
DFILE=""
PFILE=""
APIKEY=""
MODEL=""
TEMPERATURE=""
MAX_TOKENS=""
SUFFIX=""

function print_help() {
  echo "Usage: $0 {-a addr} [options]"
  echo "  -a addr: 地址"
  echo "  -p prompt: 提示前缀"
  echo "  -d data: 补全消息"
  echo "  -f prompt-file: 含提示前缀的文件"
  echo "  -F data-file: 含完整补全消息的文件"
  echo "  -k apikey: 密钥"
  echo "  -m model: 模型"
  echo "  -t temperature: 温度值"
  echo "  -M max_tokens: 最大令牌数"
  echo "  -s suffix: 后缀"
  echo "  -h: 帮助"
}
# 初始化选项
while getopts "a:p:d:f:F:k:m:s:t:M:h" opt; do
  case "$opt" in
    a)
      ADDR="$OPTARG"
      ;;
    p) 
      PROMPT="$OPTARG"
      ;;
    d)
      DATA="$OPTARG"
      ;;
    f) 
      PFILE="$OPTARG"
      ;;
    F)
      DFILE="$OPTARG"
      ;;
    k)
      APIKEY="$OPTARG"
      ;;
    m)
      MODEL="$OPTARG"
      ;;
    t)
      TEMPERATURE="$OPTARG"
      ;;
    M)
      MAX_TOKENS="$OPTARG"
      ;;
    s)
      SUFFIX="$OPTARG"
      ;;
    h)
      print_help
      exit 0
      ;;
    *) 
      echo "无效选项"
      print_help
      exit 1
      ;;
  esac
done

TEMP='{
  "model": "DeepSeek-Coder-V2-Lite-Base",
  "prompt": "#!/usr/bin/env python\n# coding: utf-8\nimport time\nimport base64\ndef trace(rsp):\n    print",
  "temperature": 0.1,
  "max_tokens": 200,
  "stop": [],
  "beta_mode": false,
  "suffix": null
}'

if [ X"$DFILE" != X"" ]; then
  DATA="$(cat $DFILE)"
elif [ X"$DATA" == X"" ]; then
  DATA="$TEMP"
fi

if [ X"$PROMPT" != X"" ]; then
  DATA=`jq --arg newValue "$PROMPT" '.prompt = $newValue' <<< "$DATA"`
elif [ X"$PFILE" != X"" ]; then
  DATA=`jq --arg newValue "$(cat $PFILE)" '.prompt = $newValue' <<< "$DATA"`
fi

if [ X"$TEMPERATURE" != X"" ]; then
  DATA=`jq --argjson newValue "$TEMPERATURE" '.temperature = $newValue' <<< "$DATA"`
fi

if [ X"$MAX_TOKENS" != X"" ]; then
  DATA=`jq --argjson newValue "$MAX_TOKENS" '.max_tokens = $newValue' <<< "$DATA"`
fi

if [ X"$MODEL" != X"" ]; then
  DATA=`jq --arg newValue "$MODEL" '.model = $newValue' <<< "$DATA"`
fi

if [ X"$SUFFIX" != X"" ]; then
  DATA=`jq --arg newValue "$SUFFIX" '.suffix = $newValue' <<< "$DATA"`
fi

if [ X"$ADDR" == X"" ]; then
  echo missing <addr>, such as: -a "http://172.16.0.4:5001/v1/completions"
  exit 1
fi

HEADERS=("-H" "Content-Type: application/json")
if [ X"$APIKEY" != X"" ]; then
  HEADERS+=("-H" "Authorization: Bearer $APIKEY")
fi

echo curl -i $ADDR "${HEADERS[@]}" -X POST -d "$DATA"
curl -i $ADDR "${HEADERS[@]}" -X POST -d "$DATA"


