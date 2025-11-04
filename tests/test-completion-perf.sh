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

# 创建结果文件
RESULT_FILE="$RESULTS_DIR/perf_results.csv"
echo "文件名,语言,响应时间(ms),状态,补全内容长度" > "$RESULT_FILE"

# 定义测试文件数组
declare -a test_files=(
    "go0.go:go"
    "go1.go:go"
    "go2.go:go"
    "go3.go:go"
    "go4.go:go"
    "go5.go:go"
    "go6.go:go"
    "go7.go:go"
    "go8.go:go"
    "go9.go:go"
    "c0.c:c"
    "c1.c:c"
    "c2.c:c"
    "c3.c:c"
    "c4.c:c"
    "c5.c:c"
    "c6.c:c"
    "c7.c:c"
    "c8.c:c"
    "c9.c:c"
    "cpp1.cpp:c++"
    "cpp2.cpp:c++"
    "cpp3.cpp:c++"
    "cpp4.cpp:c++"
    "cpp5.cpp:c++"
    "cpp6.cpp:c++"
    "cpp7.cpp:c++"
    "cpp8.cpp:c++"
    "cpp9.cpp:c++"
    "cpp10.cpp:c++"
    "10.py:python"
    "20.py:python"
    "30.py:python"
    "40.py:python"
    "50.py:python"
    "60.py:python"
    "70.py:python"
    "80.py:python"
    "90.py:python"
    "00.py:python"
    "python1.py:python"
    "python2.py:python"
    "python3.py:python"
    "python4.py:python"
    "python5.py:python"
    "python6.py:python"
    "python7.py:python"
    "python8.py:python"
    "python9.py:python"
    "python10.py:python"
    "python11.py:python"
    "java1.java:java"
    "java2.java:java"
    "java3.java:java"
    "java4.java:java"
    "java5.java:java"
    "java6.java:java"
    "java7.java:java"
    "java8.java:java"
    "java9.java:java"
    "java10.java:java"
    "javascript1.js:javascript"
    "javascript2.js:javascript"
    "javascript3.js:javascript"
    "javascript4.js:javascript"
    "javascript5.js:javascript"
    "javascript6.js:javascript"
    "javascript7.js:javascript"
    "javascript8.js:javascript"
    "javascript9.js:javascript"
    "javascript10.js:javascript"
    "typescript1.ts:typescript"
    "typescript2.ts:typescript"
    "typescript3.ts:typescript"
    "typescript4.ts:typescript"
    "typescript5.ts:typescript"
    "typescript6.ts:typescript"
    "typescript7.ts:typescript"
    "typescript8.ts:typescript"
    "typescript9.ts:typescript"
    "typescript10.ts:typescript"
    "lua1.lua:lua"
    "lua2.lua:lua"
    "lua3.lua:lua"
    "lua4.lua:lua"
    "lua5.lua:lua"
    "lua6.lua:lua"
    "lua7.lua:lua"
    "lua8.lua:lua"
    "lua9.lua:lua"
    "lua10.lua:lua"
)

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
total_tests=${#test_files[@]}
current_test=0

for file_info in "${test_files[@]}"; do
    # 分割文件名和语言
    IFS=':' read -r filename language <<< "$file_info"
    
    # 检查文件是否存在
    if [ ! -f "./data/$filename" ]; then
        echo "警告：文件 data/$filename 不存在，跳过测试"
        continue
    fi
    
    current_test=$((current_test + 1))
    # 记录开始时间
    start_time=$(date +%s%3N)  # 毫秒级时间戳
    # 执行补全请求
    response_file="$RESULTS_DIR/${filename%.*}.json"

    echo "[$current_test/$total_tests] 测试文件: ./data/$filename (语言: $language)"
    echo  "  "./completion-extend.sh -v -f "data/$filename" -l $language -a "$COMPLETION_URL" -o "$response_file"
    
    # 使用completion-extend.sh发送请求并保存响应
    if ./completion-extend.sh -v -f "data/$filename" -l $language -a "$COMPLETION_URL" -o "$response_file"; then
        # 记录结束时间
        end_time=$(date +%s%3N)
        response_time=$((end_time - start_time))
        
        # 提取状态和补全内容长度
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
        
        # 记录结果
        echo "$filename,$language,$response_time,$status,$completion_length" >> "$RESULT_FILE"
        echo "  响应时间: ${response_time}ms, 状态: $status, 补全内容长度: $completion_length"
        
        # 保存响应内容
        echo "响应内容已保存到: $response_file"
    else
        # 请求失败
        end_time=$(date +%s%3N)
        response_time=$((end_time - start_time))
        status="失败"
        completion_length="0"
        
        # 记录结果
        echo "$filename,$language,$response_time,$status,$completion_length" >> "$RESULT_FILE"
        echo "  请求失败，响应时间: ${response_time}ms"
    fi
    
    # 添加延迟，避免请求过于频繁
    sleep 1
done

echo "所有测试已完成，结果已保存到: $RESULT_FILE"

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

# 生成性能报告
echo "生成性能报告..."
report_file="$RESULTS_DIR/perf_reports.txt"

echo "LLM补全性能测试报告" > "$report_file"
echo "测试时间: $(date)" >> "$report_file"
echo "测试文件数量: $total_tests" >> "$report_file"
echo "" >> "$report_file"

# 按语言统计
echo "按语言统计:" >> "$report_file"
for lang in go c c++ python java javascript typescript lua; do
    echo "  $lang:" >> "$report_file"
    lang_count=$(grep ",$lang," "$RESULT_FILE" | wc -l)
    if [ $lang_count -gt 0 ]; then
        avg_time=$(grep ",$lang," "$RESULT_FILE" | awk -F',' '{sum+=$3; count++} END {if(count>0) print sum/count; else print 0}')
        success_count=$(grep ",$lang,.*,成功," "$RESULT_FILE" | wc -l)
        echo "    测试数量: $lang_count" >> "$report_file"
        echo "    成功数量: $success_count" >> "$report_file"
        echo "    平均响应时间: ${avg_time}ms" >> "$report_file"
    else
        echo "    无测试数据" >> "$report_file"
    fi
    echo "" >> "$report_file"
done

# 总体统计
echo "总体统计:" >> "$report_file"
total_count=$(tail -n +2 "$RESULT_FILE" | wc -l)
total_success=$(grep ",成功," "$RESULT_FILE" | wc -l)
total_avg_time=$(tail -n +2 "$RESULT_FILE" | awk -F',' '{sum+=$3; count++} END {if(count>0) print sum/count; else print 0}')

echo "  总测试数量: $total_count" >> "$report_file"
echo "  总成功数量: $total_success" >> "$report_file"
echo "  成功率: $(echo "scale=2; $total_success * 100 / $total_count" | bc)%" >> "$report_file"
echo "  平均响应时间: ${total_avg_time}ms" >> "$report_file"

echo "性能报告已保存到: $report_file"
echo "所有任务已完成，后台进程已停止。"