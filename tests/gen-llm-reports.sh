#!/bin/bash

# 生成LLM性能报告的脚本
# 从数据目录获取输入文件，根据输出结果生成CSV文件和总结报告

# 显示使用说明
show_usage() {
    echo "用法: $0 [选项]"
    echo "选项:"
    echo "  -d, --data-dir DIR   指定数据目录 (默认: ./data)"
    echo "  -o, --output DIR    指定输出目录 (默认: llm_perf_日期时间)"
    echo "  -r, --response-dir DIR  指定响应文件目录 (如果与输出目录不同)"
    echo "  -j, --json-file FILE 指定性能数据JSON文件路径"
    echo "  -h, --help          显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 -d data -o results"
    echo "  $0 --data-dir ./data --output-dir reports --response-dir responses"
    echo "  $0 -d ./data -o results -j perf_data.json"
}

# 默认参数值
DATA_DIR="data"
OUTPUT_DIR=""
RESPONSE_DIR=""
JSON_FILE=""

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
        -j|--json-file)
            JSON_FILE="$2"
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
    OUTPUT_DIR="llm_perf_$(date +%Y%m%d_%H%M%S)"
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
RESULT_FILE="$OUTPUT_DIR/perf_results.csv"
echo "文件名,语言,响应时间(ms),状态,补全内容长度,输入长度,输入Token,输出Token" > "$RESULT_FILE"

echo "处理测试结果..."
total_files=0
processed=0

# 如果提供了JSON文件，从JSON文件中读取性能数据
if [ ! -z "$JSON_FILE" ] && [ -f "$JSON_FILE" ]; then
    echo "从JSON文件读取性能数据: $JSON_FILE"
    
    # 检查jq是否可用
    if ! command -v jq &> /dev/null; then
        echo "错误: 需要安装jq工具来解析JSON文件"
        echo "请使用以下命令安装jq:"
        echo "  Ubuntu/Debian: sudo apt-get install jq"
        echo "  CentOS/RHEL: sudo yum install jq"
        echo "  macOS: brew install jq"
        exit 1
    fi
    
    # 获取JSON数组中的对象数量
    total_files=$(jq '. | length' "$JSON_FILE")
    
    # 处理每个JSON对象
    for ((i=0; i<total_files; i++)); do
        filename=$(jq -r ".[$i].filename" "$JSON_FILE")
        response_time=$(jq -r ".[$i].response_time_ms" "$JSON_FILE")
        status=$(jq -r ".[$i].status" "$JSON_FILE")
        
        # 从文件名推断语言类型
        language=$(get_language_by_extension "$filename")
        
        # 获取输入文件长度
        input_length=$(wc -c < "$DATA_DIR/$filename" 2>/dev/null || echo "0")
        
        # 响应文件路径
        response_file="$RESPONSE_DIR/${filename%.*}.json"
        
        processed=$((processed + 1))
        echo "[$processed/$total_files] 处理文件: $filename"
        
        if [ -f "$response_file" ]; then
            # 尝试从响应文件中提取信息
            if grep -q '"choices"' "$response_file"; then
                # 如果是JSON响应，提取补全内容
                completion_length=$(jq -r '.choices[0].text | length' "$response_file" 2>/dev/null || echo "0")
                
                # 提取token信息
                prompt_tokens=$(jq -r '.usage.prompt_tokens // "N/A"' "$response_file" 2>/dev/null)
                completion_tokens=$(jq -r '.usage.completion_tokens // "N/A"' "$response_file" 2>/dev/null)
                
                # 如果获取不到token信息，设置为N/A
                if [ "$prompt_tokens" = "null" ] || [ -z "$prompt_tokens" ]; then
                    prompt_tokens="N/A"
                fi
                if [ "$completion_tokens" = "null" ] || [ -z "$completion_tokens" ]; then
                    completion_tokens="N/A"
                fi
                
                # 根据JSON文件中的状态确定中文状态
                if [ "$status" = "success" ]; then
                    status_cn="成功"
                else
                    status_cn="失败"
                fi
                
                echo "  文件: $filename, 状态: $status_cn, 响应时间: ${response_time}ms"
            else
                # 如果不是JSON响应，可能是错误信息
                completion_length="0"
                prompt_tokens="N/A"
                completion_tokens="N/A"
                
                # 根据JSON文件中的状态确定中文状态
                if [ "$status" = "success" ]; then
                    status_cn="成功"
                else
                    status_cn="失败"
                fi
                
                echo "  文件: $filename, 状态: $status_cn, 响应时间: ${response_time}ms"
            fi
            
            # 记录结果到CSV
            echo "$filename,$language,$response_time,$status_cn,$completion_length,$input_length,$prompt_tokens,$completion_tokens" >> "$RESULT_FILE"
        else
            # 响应文件不存在
            completion_length="0"
            prompt_tokens="N/A"
            completion_tokens="N/A"
            
            # 根据JSON文件中的状态确定中文状态
            if [ "$status" = "success" ]; then
                status_cn="成功"
            else
                status_cn="失败"
            fi
            
            # 记录结果到CSV
            echo "$filename,$language,$response_time,$status_cn,$completion_length,$input_length,$prompt_tokens,$completion_tokens" >> "$RESULT_FILE"
            echo "  文件: $filename, 状态: $status_cn, 响应时间: ${response_time}ms"
        fi
    done
else
    # 如果没有提供JSON文件或文件不存在，使用原来的方式处理
    echo "未提供JSON文件或文件不存在，使用传统方式处理..."
    
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
        
        # 计算输入文件内容长度（字符数）
        input_length=$(wc -c < "$filepath")
        
        # 查找对应的响应文件
        response_file="$RESPONSE_DIR/${filename%.*}.json"
        
        processed=$((processed + 1))
        echo "[$processed/$total_files] 处理文件: $filename"
        
        if [ -f "$response_file" ]; then
            # 尝试从响应文件中提取信息
            if grep -q '"choices"' "$response_file"; then
                # 如果是JSON响应，提取补全内容
                completion_length=$(jq -r '.choices[0].text | length' "$response_file" 2>/dev/null || echo "0")
                
                # 提取token信息
                prompt_tokens=$(jq -r '.usage.prompt_tokens // "N/A"' "$response_file" 2>/dev/null)
                completion_tokens=$(jq -r '.usage.completion_tokens // "N/A"' "$response_file" 2>/dev/null)
                
                # 如果获取不到token信息，设置为N/A
                if [ "$prompt_tokens" = "null" ] || [ -z "$prompt_tokens" ]; then
                    prompt_tokens="N/A"
                fi
                if [ "$completion_tokens" = "null" ] || [ -z "$completion_tokens" ]; then
                    completion_tokens="N/A"
                fi
                
                # 响应时间无法从文件名获取，设置为N/A
                response_time="N/A"
                status="成功"
                
                echo "  文件: $filename, 状态: $status, 输入Token: $prompt_tokens, 输出Token: $completion_tokens"
            else
                # 如果不是JSON响应，可能是错误信息
                completion_length="0"
                prompt_tokens="N/A"
                completion_tokens="N/A"
                response_time="N/A"
                status="错误"
                echo "  文件: $filename, 状态: 错误"
            fi
            
            # 记录结果到CSV
            echo "$filename,$language,$response_time,$status,$completion_length,$input_length,$prompt_tokens,$completion_tokens" >> "$RESULT_FILE"
        else
            # 响应文件不存在
            completion_length="0"
            prompt_tokens="N/A"
            completion_tokens="N/A"
            response_time="N/A"
            status="无响应文件"
            
            # 记录结果到CSV
            echo "$filename,$language,$response_time,$status,$completion_length,$input_length,$prompt_tokens,$completion_tokens" >> "$RESULT_FILE"
            echo "  文件: $filename, 状态: 无响应文件"
        fi
    done
fi

# 生成性能报告
echo "生成性能报告..."
report_file="$OUTPUT_DIR/perf_reports.txt"

echo "LLM补全性能测试报告" > "$report_file"
echo "测试时间: $(date)" >> "$report_file"
echo "数据来源: $DATA_DIR" >> "$report_file"
echo "响应文件目录: $RESPONSE_DIR" >> "$report_file"
echo "测试文件数量: $processed" >> "$report_file"
echo "" >> "$report_file"

# 获取结果中出现的所有语言类型
languages=$(tail -n +2 "$RESULT_FILE" | awk -F',' '{print $2}' | sort | uniq)

# 按语言统计
echo "按语言统计:" >> "$report_file"
for lang in $languages; do
    echo "  $lang:" >> "$report_file"
    lang_count=$(grep ",$lang," "$RESULT_FILE" | wc -l)
    if [ $lang_count -gt 0 ]; then
        success_count=$(grep ",$lang,.*,成功," "$RESULT_FILE" | wc -l)
        
        # 计算Token统计
        total_prompt_tokens=$(grep ",$lang,.*,成功," "$RESULT_FILE" | awk -F',' '{if($7!="N/A") sum+=$7} END {print sum+0}')
        total_completion_tokens=$(grep ",$lang,.*,成功," "$RESULT_FILE" | awk -F',' '{if($8!="N/A") sum+=$8} END {print sum+0}')
        
        echo "    测试数量: $lang_count" >> "$report_file"
        echo "    成功数量: $success_count" >> "$report_file"
        echo "    总输入Token: $total_prompt_tokens" >> "$report_file"
        echo "    总输出Token: $total_completion_tokens" >> "$report_file"
    else
        echo "    无测试数据" >> "$report_file"
    fi
    echo "" >> "$report_file"
done

# 总体统计
echo "总体统计:" >> "$report_file"
total_count=$(tail -n +2 "$RESULT_FILE" | wc -l)
total_success=$(grep ",成功," "$RESULT_FILE" | wc -l)

# 计算总体Token统计
total_prompt_tokens=$(grep ",成功," "$RESULT_FILE" | awk -F',' '{if($7!="N/A") sum+=$7} END {print sum+0}')
total_completion_tokens=$(grep ",成功," "$RESULT_FILE" | awk -F',' '{if($8!="N/A") sum+=$8} END {print sum+0}')

echo "  总测试数量: $total_count" >> "$report_file"
echo "  总成功数量: $total_success" >> "$report_file"
if [ $total_count -gt 0 ]; then
    echo "  成功率: $(echo "scale=2; $total_success * 100 / $total_count" | bc)%" >> "$report_file"
else
    echo "  成功率: 0%" >> "$report_file"
fi
echo "  总输入Token: $total_prompt_tokens" >> "$report_file"
echo "  总输出Token: $total_completion_tokens" >> "$report_file"

echo "CSV结果文件已保存到: $RESULT_FILE"
echo "性能报告已保存到: $report_file"
echo "报告生成完成。"