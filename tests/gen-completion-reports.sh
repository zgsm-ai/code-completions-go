#!/bin/bash

# 生成补全性能报告的脚本
# 从数据目录获取输入文件，根据输出结果生成CSV文件和总结报告
# 从verbose.output.usage中获取token计数

# 显示使用说明
show_usage() {
    echo "用法: $0 [选项]"
    echo "选项:"
    echo "  -d, --data-dir DIR   指定数据目录 (默认: tests/data)"
    echo "  -o, --output DIR    指定输出目录 (默认: completion_perf_日期时间)"
    echo "  -r, --response-dir DIR  指定响应文件目录 (如果与输出目录不同)"
    echo "  -h, --help          显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 -d tests/data -o results"
    echo "  $0 --data-dir ./data --output-dir reports --response-dir responses"
}

# 默认参数值
DATA_DIR="tests/data"
OUTPUT_DIR=""
RESPONSE_DIR=""

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--data-dir)
            DATA_DIR="$2"
            shift 2
            ;;
        -o|--output)
            OUTPUT_DIR="$2"
            shift 2
            ;;
        -r|--response-dir)
            RESPONSE_DIR="$2"
            shift 2
            ;;
        -h|--help)
            show_usage
            exit 0
            ;;
        *)
            echo "错误: 未知选项 $1"
            show_usage
            exit 1
            ;;
    esac
done

# 检查数据目录是否存在
if [ ! -d "$DATA_DIR" ]; then
    echo "错误: 数据目录不存在: $DATA_DIR"
    exit 1
fi

# 如果未指定输出目录，创建带时间戳的目录
if [ -z "$OUTPUT_DIR" ]; then
    OUTPUT_DIR="completion_perf_$(date +%Y%m%d_%H%M%S)"
fi

# 创建输出目录
mkdir -p "$OUTPUT_DIR"

# 如果未指定响应文件目录，使用输出目录
if [ -z "$RESPONSE_DIR" ]; then
    RESPONSE_DIR="$OUTPUT_DIR"
fi

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

# 创建结果文件
result_file="$OUTPUT_DIR/perf_results.csv"
echo "文件名,语言,响应时间(ms),状态,补全内容长度,输入长度,输入Token,输出Token" > "$result_file"

echo "处理测试结果..."
processed=0

# 初始化语言统计变量
declare -A lang_count      # 各语言测试数量
declare -A lang_success    # 各语言成功数量
declare -A lang_time_sum   # 各语言响应时间总和
declare -A lang_time_valid # 各语言有效响应时间计数
declare -A lang_prompt_tokens    # 各语言输入Token总和
declare -A lang_completion_tokens # 各语言输出Token总和
declare -A languages_seen  # 记录已处理的语言

# 总体统计变量
total_count=0
total_success=0
total_time_sum=0
total_time_valid=0
total_prompt_tokens=0
total_completion_tokens=0

# 获取数据目录中的所有文件
file_list=$(find "$DATA_DIR" -type f | sort)
total_files=$(echo "$file_list" | wc -l)

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
    
    # 记录已处理的语言
    languages_seen["$language"]=1
    
    # 计算输入文件内容长度（字符数）
    input_length=$(wc -c < "$filepath")
    
    # 查找对应的响应文件
    response_file="$RESPONSE_DIR/${filename%.*}.json"
    
    processed=$((processed + 1))
    echo "[$processed/$total_files] 处理文件: $filename (语言: $language)"
    
    # 初始化统计值
    completion_length="0"
    prompt_tokens="N/A"
    completion_tokens="N/A"
    response_time="N/A"
    status="无响应文件"
    
    if [ -f "$response_file" ]; then
        # 尝试从响应文件中提取信息
        if grep -q '"choices"' "$response_file"; then
            # 如果是JSON响应，提取补全内容
            completion_length=$(jq -r '.choices[0].text | length' "$response_file" 2>/dev/null || echo "0")
            
            # 从verbose.output.usage中提取token信息
            prompt_tokens=$(jq -r '.verbose.output.usage.prompt_tokens // "N/A"' "$response_file" 2>/dev/null)
            completion_tokens=$(jq -r '.verbose.output.usage.completion_tokens // "N/A"' "$response_file" 2>/dev/null)
            
            # 从usage.total_duration中提取响应时间（单位是毫秒）
            response_time=$(jq -r '.usage.total_duration // "N/A"' "$response_file" 2>/dev/null)
            
            # 如果获取不到token信息，设置为N/A
            if [ "$prompt_tokens" = "null" ] || [ -z "$prompt_tokens" ]; then
                prompt_tokens="N/A"
            fi
            if [ "$completion_tokens" = "null" ] || [ -z "$completion_tokens" ]; then
                completion_tokens="N/A"
            fi
            if [ "$response_time" = "null" ] || [ -z "$response_time" ]; then
                response_time="N/A"
            fi
            
            status="成功"
            
            echo "  文件: $filename, 状态: $status, 输入Token: $prompt_tokens, 输出Token: $completion_tokens"
        else
            # 如果不是JSON响应，可能是错误信息
            status="错误"
            echo "  文件: $filename, 状态: 错误"
        fi
    fi
    
    # 更新语言统计
    lang_count["$language"]=$((${lang_count["$language"]:-0} + 1))
    total_count=$((total_count + 1))
    
    if [ "$status" = "成功" ]; then
        lang_success["$language"]=$((${lang_success["$language"]:-0} + 1))
        total_success=$((total_success + 1))
        
        # 更新响应时间统计
        if [ "$response_time" != "N/A" ]; then
            lang_time_sum["$language"]=$(echo "${lang_time_sum["$language"]:-0} + $response_time" | bc)
            lang_time_valid["$language"]=$((${lang_time_valid["$language"]:-0} + 1))
            total_time_sum=$(echo "$total_time_sum + $response_time" | bc)
            total_time_valid=$((total_time_valid + 1))
        fi
        
        # 更新Token统计
        if [ "$prompt_tokens" != "N/A" ]; then
            lang_prompt_tokens["$language"]=$(echo "${lang_prompt_tokens["$language"]:-0} + $prompt_tokens" | bc)
            total_prompt_tokens=$(echo "$total_prompt_tokens + $prompt_tokens" | bc)
        fi
        
        if [ "$completion_tokens" != "N/A" ]; then
            lang_completion_tokens["$language"]=$(echo "${lang_completion_tokens["$language"]:-0} + $completion_tokens" | bc)
            total_completion_tokens=$(echo "$total_completion_tokens + $completion_tokens" | bc)
        fi
    fi
    
    # 记录结果到CSV
    echo "$filename,$language,$response_time,$status,$completion_length,$input_length,$prompt_tokens,$completion_tokens" >> "$result_file"
done

# 生成性能报告
echo "生成性能报告..."
report_file="$OUTPUT_DIR/perf_reports.txt"

echo "补全性能测试报告" > "$report_file"
echo "测试时间: $(date)" >> "$report_file"
echo "数据来源: $DATA_DIR" >> "$report_file"
echo "响应文件目录: $RESPONSE_DIR" >> "$report_file"
echo "测试文件数量: $processed" >> "$report_file"
echo "" >> "$report_file"

# 按语言统计（使用已收集的统计变量）
echo "按语言统计:" >> "$report_file"
for lang in "${!languages_seen[@]}"; do
    echo "  $lang:" >> "$report_file"
    l_count=${lang_count["$lang"]:-0}
    if [ $l_count -gt 0 ]; then
        l_success=${lang_success["$lang"]:-0}
        l_time_valid=${lang_time_valid["$lang"]:-0}
        l_time_sum=${lang_time_sum["$lang"]:-0}
        l_prompt_tokens=${lang_prompt_tokens["$lang"]:-0}
        l_completion_tokens=${lang_completion_tokens["$lang"]:-0}
        
        # 计算平均响应时间（只计算成功的）
        if [ $l_time_valid -gt 0 ]; then
            avg_time=$(echo "scale=2; $l_time_sum / $l_time_valid" | bc)
        else
            avg_time="0"
        fi
        
        echo "    测试数量: $l_count" >> "$report_file"
        echo "    成功数量: $l_success" >> "$report_file"
        echo "    平均响应时间: ${avg_time}ms" >> "$report_file"
        echo "    总输入Token: $l_prompt_tokens" >> "$report_file"
        echo "    总输出Token: $l_completion_tokens" >> "$report_file"
    else
        echo "    无测试数据" >> "$report_file"
    fi
    echo "" >> "$report_file"
done

# 总体统计（使用已收集的总体变量）
echo "总体统计:" >> "$report_file"
echo "  总测试数量: $total_count" >> "$report_file"
echo "  总成功数量: $total_success" >> "$report_file"

if [ $total_count -gt 0 ]; then
    success_rate=$(echo "scale=2; $total_success * 100 / $total_count" | bc)
    echo "  成功率: ${success_rate}%" >> "$report_file"
else
    echo "  成功率: 0%" >> "$report_file"
fi

if [ $total_time_valid -gt 0 ]; then
    total_avg_time=$(echo "scale=2; $total_time_sum / $total_time_valid" | bc)
else
    total_avg_time="0"
fi

echo "  平均响应时间: ${total_avg_time}ms" >> "$report_file"
echo "  总输入Token: $total_prompt_tokens" >> "$report_file"
echo "  总输出Token: $total_completion_tokens" >> "$report_file"

echo "CSV结果文件已保存到: $result_file"
echo "性能报告已保存到: $report_file"
echo "报告生成完成。"