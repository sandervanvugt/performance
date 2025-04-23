#!/bin/bash

# Read total RAM and Swap from /proc/meminfo (in kB)
mem_total_kb=$(grep MemTotal /proc/meminfo | awk '{print $2}')
swap_total_kb=$(grep SwapTotal /proc/meminfo | awk '{print $2}')

# Read overcommit_ratio
overcommit_ratio=$(cat /proc/sys/vm/overcommit_ratio)

# Calculate limit in kB
allowed_kb=$(( (mem_total_kb * overcommit_ratio / 100) + swap_total_kb ))

# Convert to MB and GB
allowed_mb=$(( allowed_kb / 1024 ))
allowed_gb=$(( allowed_kb / 1024 / 1024 ))

echo "📊 System Memory Summary:"
echo "-------------------------"
echo "RAM Total     : $((mem_total_kb / 1024)) MB"
echo "Swap Total    : $((swap_total_kb / 1024)) MB"
echo "Overcommit %  : ${overcommit_ratio}%"
echo
echo "🧠 Max Virtual Memory Allocation Limit:"
echo "--------------------------------------"
echo "$allowed_kb kB"
echo "$allowed_mb MB"
echo "$allowed_gb GB"

