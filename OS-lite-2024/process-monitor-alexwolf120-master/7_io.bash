#!/bin/bash

declare -A before after name
pids=()

for pid in $(ps -eo pid); do
  if [ -r /proc/$pid/io ]; then
    before[$pid]=$(grep "rchar" /proc/$pid/io | awk '{print $2}')
    pids+=($pid)
    name[$pid]=$(xargs -0 < /proc/$pid/cmdline)
  fi
done

sleep 10

for pid in "${pids[@]}"; do
  if [ -r /proc/$pid/io ]; then
    after[$pid]=$(grep "rchar" /proc/$pid/io | awk '{print $2}')
done

for pid in "${pids[@]}"; do
  if [ -n "${after[$pid]}" ] && [ -n "${before[$pid]}" ]; then
    diff=$((after[$pid] - before[$pid]))
    echo "$pid $diff ${name[$pid]}"
  fi
done | sort -nr -k 2 | head -n 3 | awk '{print $1":"$2":"$3}' >> seven_out
