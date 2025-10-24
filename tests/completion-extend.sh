#!/bin/sh

ADDR="http://10.20.19.2:32088/code-completion/api/v1/completions"
DATA=""
PROMPT=""
DFILE=""
PFILE=""
APIKEY=""
MODEL=""
TEMPERATURE=""
MAX_TOKENS=""
COMPLETION_ID=""
CLIENT_ID=""
PROJECT_PATH=""
FILE_PROJECT_PATH=""
LANGUAGE_ID=""
ECHO=""
STREAM=""
TRIGGER_MODE=""

function print_help() {
  echo "Usage: $0 [-a addr] [options]"
  echo "  -a addr: 地址"
  echo "  -p prompt: 提示前缀"
  echo "  -d data: 补全消息"
  echo "  -f prompt-file: 含提示前缀的文件"
  echo "  -F data-file: 含完整补全消息的文件"
  echo "  -k apikey: 密钥"
  echo "  -m model: 模型"
  echo "  -t temperature: 温度值"
  echo "  -M max_tokens: 最大令牌数"
  echo "  -i completion_id: 补全ID"
  echo "  -c client_id: 客户端ID"
  echo "  -P project_path: 项目路径"
  echo "  -C file_project_path: 文件项目路径"
  echo "  -l language_id: 语言ID"
  echo "  -r trigger_mode: 触发模式"
  echo "  -e: 开启回显"
  echo "  -s: 开启流式输出"
  echo "  -h: 帮助"
}
# 初始化选项
while getopts "a:p:d:f:F:k:m:i:c:P:C:l:t:M:r:esh" opt; do
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
    i)
      COMPLETION_ID="$OPTARG"
      ;;
    c)
      CLIENT_ID="$OPTARG"
      ;;
    P)
      PROJECT_PATH="$OPTARG"
      ;;
    C)
      FILE_PROJECT_PATH="$OPTARG"
      ;;
    l)
      LANGUAGE_ID="$OPTARG"
      ;;
    r)
      TRIGGER_MODE="$OPTARG"
      ;;
    e)
      ECHO="true"
      ;;
    s)
      STREAM="true"
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
  "stream": false,
  "echo": false,
  "language_id": "python",
  "trigger_mode": "manual",
  "project_path": "g:/tmp/projects/c/redis",
  "file_project_path": "g:/tmp/projects/c/redis/src/ae.c",
  "client_id": "zbc-test",
  "completion_id": "666-94131415"
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

if [ X"$CLIENT_ID" != X"" ]; then
  DATA=`jq --arg newValue "$CLIENT_ID" '.client_id = $newValue' <<< "$DATA"`
fi

if [ X"$COMPLETION_ID" != X"" ]; then
  DATA=`jq --arg newValue "$COMPLETION_ID" '.completion_id = $newValue' <<< "$DATA"`
else
  COMPLETION_ID="666-94131415"
fi

if [ X"$PROJECT_PATH" != X"" ]; then
  DATA=`jq --arg newValue "$PROJECT_PATH" '.project_path = $newValue' <<< "$DATA"`
fi

if [ X"$FILE_PROJECT_PATH" != X"" ]; then
  DATA=`jq --arg newValue "$FILE_PROJECT_PATH" '.file_project_path = $newValue' <<< "$DATA"`
fi

if [ X"$LANGUAGE_ID" != X"" ]; then
  DATA=`jq --arg newValue "$LANGUAGE_ID" '.language_id = $newValue' <<< "$DATA"`
fi

if [ X"$TRIGGER_MODE" != X"" ]; then
  DATA=`jq --arg newValue "$TRIGGER_MODE" '.trigger_mode = $newValue' <<< "$DATA"`
fi

if [ X"$ECHO" != X"" ]; then
  DATA=`jq '.echo = true' <<< "$DATA"`
fi

if [ X"$STREAM" != X"" ]; then
  DATA=`jq '.stream = true' <<< "$DATA"`
fi

if [ X"$ADDR" == X"" ]; then
  echo missing <addr>, such as: -a "http://172.16.0.4:5001/v1/completions"
  exit 1
fi

HEADERS=("-H" "Content-Type: application/json" "-H" "X-Costrict-Version: 2.0.0" "-H" "X-Request-Id: $COMPLETION_ID")
if [ X"$APIKEY" != X"" ]; then
  HEADERS+=("-H" "Authorization: Bearer $APIKEY")
fi

echo curl -i $ADDR "${HEADERS[@]}" -X POST -d "$DATA"
curl -i $ADDR "${HEADERS[@]}" -X POST -d "$DATA"


