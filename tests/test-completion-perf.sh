#!/bin/bash

# 测试LLM补全性能的脚本
# 以串行方式测试补全模型对各种代码文件进行补全时的效果和性能

# 检查必要文件是否存在
if [ ! -f "./completion-extend.sh" ]; then
    echo "错误：找不到 completion-extend.sh 文件"
    exit 1
fi

if [ ! -f "./expose-completion.sh" ]; then
    echo "错误：找不到 expose-completion.sh 文件"
    exit 1
fi

# 补全服务的URL
COMPLETION_URL=""

# 如果存在./.env文件，则加载它
if [ -f "./.env" ]; then
    echo "加载./.env文件中的环境变量..."
    source ./.env
fi

# 创建结果目录
RESULTS_DIR="completion_perf_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$RESULTS_DIR"


# 函数：根据文件扩展名确定语言类型
get_language_by_extension() {
    local filename="$1"
    local extension="${filename##*.}"
    
    case "$extension" in
        "go") echo "go" ;;
        "c") echo "c" ;;
        "cpp"|"cc"|"cxx") echo "c++" ;;
        "py") echo "python" ;;
        "java") echo "java" ;;
        "js") echo "javascript" ;;
        "ts") echo "typescript" ;;
        "lua") echo "lua" ;;
        *) echo "unknown" ;;
    esac
}

# 检查 32088 端口是否已经被占用
echo "检查 32088 端口状态..."
if netstat -tuln | grep -q ":32088 "; then
    echo "端口 32088 已被占用，跳过 expose-completion.sh 的执行"
    EXPOSE_PID=""
else
    echo "端口 32088 未被占用，启动 expose-completion.sh 后台进程..."
    ./expose-completion.sh &
    EXPOSE_PID=$!

    # 等待一下，确保 expose-completion.sh 启动完成
    sleep 3

    # 检查 expose-completion.sh 是否成功启动
    if ! kill -0 $EXPOSE_PID 2>/dev/null; then
        echo "错误：expose-completion.sh 启动失败"
        exit 1
    fi

    echo "expose-completion.sh 已启动，PID: $EXPOSE_PID"
fi

# 串行执行测试
echo "开始串行执行LLM补全性能测试..."

# 获取数据目录中的所有文件
file_list=$(find ./data -type f | sort)
total_files=$(echo "$file_list" | wc -l)
current_test=0

for filepath in $file_list; do
    # 获取文件名（不包含路径）
    filename=$(basename "$filepath")
    
    # 根据文件扩展名确定语言类型
    language=$(get_language_by_extension "$filename")
    
    # 跳过未知文件类型
    if [ "$language" = "unknown" ]; then
        echo "跳过未知文件类型: $filename"
        continue
    fi
    
    current_test=$((current_test + 1))
    # 记录开始时间
    start_time=$(date +%s%3N)  # 毫秒级时间戳
    # 执行补全请求
    response_file="$RESULTS_DIR/${filename%.*}.json"

    echo "[$current_test/$total_files] 测试文件: $filename"
    echo  "  "./completion-extend.sh -v -f "$filepath" -l $language -a "$COMPLETION_URL" -o "$response_file"
    
    # 使用completion-extend.sh发送请求并保存响应
    if ./completion-extend.sh -v -f "$filepath" -l $language -a "$COMPLETION_URL" -o "$response_file"; then
        # 记录结束时间
        end_time=$(date +%s%3N)
        response_time=$((end_time - start_time))
        status="成功"
        
        # 尝试从响应中提取补全内容长度
        if grep -q '"choices"' "$response_file"; then
            # 如果是JSON响应，提取补全内容
            completion_length=$(jq -r '.choices[0].text | length' "$response_file" 2>/dev/null || echo "0")
        else
            # 如果不是JSON响应，可能是错误信息
            completion_length="0"
            if grep -q "error\|Error\|ERROR" "$response_file"; then
                status="错误"
            fi
        fi
        
        echo "  响应时间: ${response_time}ms, 状态: $status, 补全内容长度: $completion_length"
    else
        # 请求失败
        end_time=$(date +%s%3N)
        response_time=$((end_time - start_time))
        status="失败"
        completion_length="0"
        
        echo "  请求失败，响应时间: ${response_time}ms"
    fi
    
    # 添加延迟，避免请求过于频繁
    sleep 1
done

echo "所有测试已完成，响应文件已保存到: $RESULTS_DIR"

# 停止后台执行的 expose-completion.sh（如果存在）
if [ -n "$EXPOSE_PID" ]; then
    echo "停止 expose-completion.sh 进程..."
    if kill $EXPOSE_PID 2>/dev/null; then
        echo "expose-completion.sh 进程已停止"
    else
        echo "警告：无法停止 expose-completion.sh 进程"
    fi
else
    echo "expose-completion.sh 未由本脚本启动，无需停止"
fi

# 调用gen-completion-reports.sh生成报告
echo "使用gen-completion-reports.sh生成性能报告..."
if [ -f "./gen-completion-reports.sh" ]; then
    ./gen-completion-reports.sh -d data -o "$RESULTS_DIR" -r "$RESULTS_DIR"
    echo "性能报告生成完成"
else
    echo "错误：找不到gen-completion-reports.sh文件，无法生成报告"
fi

echo "所有任务已完成，后台进程已停止。"