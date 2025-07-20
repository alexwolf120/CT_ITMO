#!/bin/bash

mx_pid=0
mx_mem=0

for pid in $(ps axo pid --no-headers)
do
    if [[ -f /proc/$pid/status ]]
    then
        mem=$(awk '$1 == "VmRSS:" {print $2}' /proc/$pid/status)
        if [[ $mem -gt $mx_mem ]]
        then
            mx_pid=$pid
            mx_mem=$mem
        fi
    fi
done

echo "Max Pid from /proc: $mx_pid"

get_max_rss_from_top() {
    top_pid=$(top -b -n 1 | grep -E '^ *[0-9]+' | sort -k10 -nr | head -n 1 | awk '{print $1}')
    echo "$top_pid"
}
top_pid=$(get_max_rss_from_top)
echo "Max Pid from top: $top_pid"

if [ "$mx_pid" -eq "$top_pid" ]; then
    echo "Результаты совпадают."
else
    echo "Результаты не совпадают."
fi
