#!/bin/bash

# 等待多个进程完成，带超时
# 参数: $1=超时秒数(默认10秒), $2+=进程ID数组
wait_all_with_timeout() {
    local timeout=${1:-10}
    shift
    local pids=("$@")
    local start_time=$(date +%s)
    local completed_pids=()
    local remaining_pids=("${pids[@]}")
    
    echo "开始等待 ${#pids[@]} 个进程完成，超时时间: ${timeout}秒..."
    
    while [ $(( $(date +%s) - start_time )) -lt $timeout ] && [ ${#remaining_pids[@]} -gt 0 ]; do
        # 每秒检查一次
        sleep 1
        local current_time=$(date +%s)
        local elapsed=$((current_time - start_time))
        local remaining_time=$((timeout - elapsed))
        
        # 检查哪些进程已经退出
        local new_remaining_pids=()
        for pid in "${remaining_pids[@]}"; do
            if kill -0 $pid 2>/dev/null; then
                # 进程还在运行
                new_remaining_pids+=($pid)
            else
                # 进程已退出
                completed_pids+=($pid)
                local cmd="${COMMANDS[$pid]}"
                echo "进程 $pid 已自然退出 (耗时: ${elapsed}秒), 命令行: $cmd"
            fi
        done
        remaining_pids=("${new_remaining_pids[@]}")
        
        echo "第${elapsed}秒, 已完成: ${#completed_pids[@]}个, 剩余运行进程: ${#remaining_pids[@]}个(${remaining_pids[@]})"
    done
    
    # 检查是否超时
    if [ ${#remaining_pids[@]} -gt 0 ]; then
        echo "超时！终止剩余的 ${#remaining_pids[@]} 个进程..."
        for pid in "${remaining_pids[@]}"; do
            local cmd="${COMMANDS[$pid]}"
            echo "终止超时进程: PID=$pid, 命令行: $cmd"
            kill -9 $pid 2>/dev/null
        done
        return 124
    else
        echo "所有进程在超时时间内完成"
        return 0
    fi
}

# 检查必要文件是否存在
if [ ! -f "./expose-completion.sh" ]; then
    echo "错误：找不到 expose-completion.sh 文件"
    exit 1
fi

if [ ! -f "./completion-extend.sh" ]; then
    echo "错误：找不到 completion-extend.sh 文件"
    exit 1
fi

# 创建结果目录（如果不存在）
RESULTS_DIR="./concurrent_$(date +%Y%m%d_%H%M%S)"
if [ ! -d "$RESULTS_DIR" ]; then
    echo "创建 $RESULTS_DIR 目录..."
    mkdir -p $RESULTS_DIR
fi

# 定义参数数组，每个元素是一组参数
declare -a params=(
    "-i comp-00 -f data/00.py"
    "-i comp-10 -f data/10.py"
    "-i comp-20 -f data/20.py"
    "-i comp-30 -f data/30.py"
    "-i comp-40 -f data/40.py"
    "-i comp-50 -f data/50.py"
    "-i comp-60 -f data/60.py"
    "-i comp-70 -f data/70.py"
    "-i comp-80 -f data/80.py"
    "-i comp-90 -f data/90.py"
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
    sleep 2

    # 检查 expose-completion.sh 是否成功启动
    if ! kill -0 $EXPOSE_PID 2>/dev/null; then
        echo "错误：expose-completion.sh 启动失败"
        exit 1
    fi

    echo "expose-completion.sh 已启动，PID: $EXPOSE_PID"
fi

# 并发执行 completion-extend.sh，使用数组中的参数
echo "开始并发执行 completion-extend.sh..."
declare -a PIDS=()
declare -a COMMANDS=()
for ((i=0; i<${#params[@]}; i++)); do
    # 为每个任务生成唯一的输出文件名
    output_file="$RESULTS_DIR/task_${i}.txt"

    echo "执行第 $((i+1)) 个任务，参数: ${params[$i]}, 输出到: $output_file"
    ./completion-extend.sh ${params[$i]} -o "$output_file" &
    pid=$!
    PIDS+=($pid)
    COMMANDS[$pid]="./completion-extend.sh ${params[$i]} -o $output_file"
    sleep 0.2
done

echo "所有任务已启动，等待完成..."
echo "启动的进程PID: ${PIDS[@]}"

# 等待所有 completion-extend.sh 进程完成，最多10秒超时
wait_all_with_timeout 10 "${PIDS[@]}"

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

echo "所有任务已完成，后台进程已停止。"

# 显示生成的报告文件
echo ""
echo "生成的报告文件："
if [ -d "$RESULTS_DIR" ]; then
    ls -la $RESULTS_DIR/task_*.txt 2>/dev/null | while read line; do
        echo "  $line"
    done
    report_count=$(ls -1 $RESULTS_DIR/task_*.txt 2>/dev/null | wc -l)
    echo "总计: $report_count 个报告文件保存在 $RESULTS_DIR/ 目录中"
else
    echo "  未找到报告文件"
fi