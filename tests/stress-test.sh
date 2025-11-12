#!/bin/bash

# Stress test script for completion API
# This script generates multiple concurrent requests to test the system performance

# Default values
COMPLETION_URL=""
API_KEY=""
DATA_DIR="./data"
CONCURRENT_JOBS=10
CONCURRENT_CLIENTS=10
MODEL=""
OUTPUT_DIR="stress_$(date +%Y%m%d_%H%M%S)"
TEMPERATURE="0.1"
MAX_TOKENS="50"

# 如果存在./.env文件，则加载它
if [ -f "./.env" ]; then
    echo "加载./.env文件中的环境变量..."
    source ./.env
fi

# Generate random 16-byte hex string
function generate_random_id() {
  openssl rand -hex 16 2>/dev/null || (
    # Fallback if openssl is not available
    date +%s%N | sha256sum | head -c 32
  )
}

# Export function for use in subshells
export -f generate_random_id

# Print usage information
function print_help() {
  echo "Usage: $0 [options]"
  echo "  -u addr: 服务器地址 (必需)"
  echo "  -d dir: 测试文件目录 (默认: data)"
  echo "  -j jobs: 并发任务数 (默认: 10)"
  echo "  -c clients: 客户端ID数量 (默认: 10)"
  echo "  -k apikey: API密钥"
  echo "  -m model: 模型名称"
  echo "  -o dir: 输出目录 (默认: stress_xxx)"
  echo "  -t temperature: 温度值 (默认: 0.1)"
  echo "  -M max_tokens: 最大令牌数 (默认: 50)"
  echo "  -h: 帮助信息"
}

# Parse command line options
while getopts "u:d:j:c:k:m:o:t:M:h" opt; do
  case "$opt" in
    u)
      COMPLETION_URL="$OPTARG"
      ;;
    d)
      DATA_DIR="$OPTARG"
      ;;
    j)
      CONCURRENT_JOBS="$OPTARG"
      ;;
    c)
      CONCURRENT_CLIENTS="$OPTARG"
      ;;
    k)
      API_KEY="$OPTARG"
      ;;
    m)
      MODEL="$OPTARG"
      ;;
    o)
      OUTPUT_DIR="$OPTARG"
      ;;
    t)
      TEMPERATURE="$OPTARG"
      ;;
    M)
      MAX_TOKENS="$OPTARG"
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

# Validate required parameters
if [ -z "$COMPLETION_URL" ]; then
  echo "错误: 缺少必需参数 '-u/--addr'"
  print_help
  exit 1
fi

# Check if data directory exists
if [ ! -d "$DATA_DIR" ]; then
  echo "错误: 数据目录 '$DATA_DIR' 不存在"
  exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Generate ClientIDs based on CONCURRENT_CLIENTS
echo "生成ClientID列表..."
CLIENT_IDS=()
for i in $(seq 1 $CONCURRENT_CLIENTS); do
  CLIENT_ID=$(generate_random_id)
  CLIENT_IDS+=("cli-$CLIENT_ID")
  echo "ClientID $i: $CLIENT_ID"
done

# Get list of test files
echo "扫描测试文件..."
TEST_FILES=($(find "$DATA_DIR" -type f \( -name "*.py" -o -name "*.c" -o -name "*.cpp" -o -name "*.go" -o -name "*.java" -o -name "*.js" -o -name "*.ts" -o -name "*.lua" \) | sort))

if [ ${#TEST_FILES[@]} -eq 0 ]; then
  echo "错误: 在 '$DATA_DIR' 目录中未找到测试文件"
  exit 1
fi

echo "找到 ${#TEST_FILES[@]} 个测试文件"

# Function to execute a single request
function execute_request() {
  local file="$1"
  local index="$2"
  local client_id="${CLIENT_IDS[$(($RANDOM % $CONCURRENT_CLIENTS))]}"
  local completion_id="comp-$(generate_random_id)"
  local fname=$(basename "$file")
  local output_file="$OUTPUT_DIR/${fname%.*}.json"

  # Determine file extension for language_id
  local ext="${file##*.}"
  local language_id=""
  case "$ext" in
    py) language_id="python" ;;
    c) language_id="c" ;;
    cpp) language_id="cpp" ;;
    go) language_id="go" ;;
    java) language_id="java" ;;
    js) language_id="javascript" ;;
    ts) language_id="typescript" ;;
    lua) language_id="lua" ;;
    *) language_id="text" ;;
  esac
  
  # Prepare command arguments
  local cmd_args="-a $COMPLETION_URL -f $file -c $client_id -i $completion_id -l $language_id -t $TEMPERATURE -M $MAX_TOKENS -o $output_file -n"
  
  if [ -n "$API_KEY" ]; then
    cmd_args="$cmd_args -k $API_KEY"
  fi
  
  if [ -n "$MODEL" ]; then
    cmd_args="$cmd_args -m $MODEL"
  fi
  
  # Execute the request
  echo "执行请求 $index: 文件=$file, ClientID=$client_id, CompletionID=$completion_id"
  ./completion-via-service.sh $cmd_args
  
  # Check if request was successful
  if [ $? -eq 0 ]; then
    echo "请求 $index 完成: 输出文件=$output_file"
  else
    echo "请求 $index 失败: 文件=$file"
  fi
}

# Start time for performance measurement
START_TIME=$(date +%s)

# Export functions and variables for subshells
export -f execute_request
export CLIENT_IDS
export OUTPUT_DIR
export COMPLETION_URL
export API_KEY
export MODEL
export TEMPERATURE
export MAX_TOKENS

# Execute concurrent requests
echo "开始并发测试，并发数: $CONCURRENT_JOBS"
echo "测试开始时间: $(date)"

# Use a for loop with background processes for parallel execution
for i in "${!TEST_FILES[@]}"; do
  (
    index=$((i+1))
    file="${TEST_FILES[$i]}"
    execute_request "$file" "$index"
  ) &

  # Control the number of concurrent jobs
  if (( $((i+1)) % $CONCURRENT_JOBS == 0 )); then
    wait
  fi
done

# Wait for any remaining background processes
wait

# End time for performance measurement
END_TIME=$(date +%s)
ELAPSED_TIME=$((END_TIME - START_TIME))

echo "测试完成时间: $(date)"
echo "总耗时: $ELAPSED_TIME 秒"
echo "结果文件保存在: $OUTPUT_DIR"

# Generate summary report
REPORT_FILE="$OUTPUT_DIR/stress_summary.txt"
echo "生成测试报告: $REPORT_FILE"

cat > "$REPORT_FILE" << EOF
压力测试报告
=============

测试时间: $(date)
测试持续时间: $ELAPSED_TIME 秒
并发任务数: $CONCURRENT_JOBS
测试文件数: ${#TEST_FILES[@]}
服务器地址: $COMPLETION_URL

ClientID列表:
EOF

for i in "${!CLIENT_IDS[@]}"; do
  echo "ClientID $((i+1)): ${CLIENT_IDS[i]}" >> "$REPORT_FILE"
done

# 调用gen-completion-reports.sh生成CSV文件和性能报告
echo "使用gen-completion-reports.sh生成CSV文件和性能报告..."
if [ -f "./gen-completion-reports.sh" ]; then
    ./gen-completion-reports.sh -d "$DATA_DIR" -r "$OUTPUT_DIR" -o "$OUTPUT_DIR"
    echo "性能报告生成完成"
    
    # 显示生成的报告文件
    echo ""
    echo "生成的报告文件:"
    if [ -f "$OUTPUT_DIR/perf_results.csv" ]; then
        echo "- CSV结果文件: $OUTPUT_DIR/perf_results.csv"
        echo "前5行内容:"
        head -n 5 "$OUTPUT_DIR/perf_results.csv" | while IFS=, read -r filename status fulltime prompt_tokens completion_tokens total_duration; do
            echo "  $filename | $status | $fulltime | $prompt_tokens | $completion_tokens | $total_duration"
        done
    fi
else
    echo "警告: 找不到gen-completion-reports.sh脚本，跳过报告生成"
fi