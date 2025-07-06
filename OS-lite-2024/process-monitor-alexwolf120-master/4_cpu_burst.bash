#!/bin/bash

for pid in $(ps -eo pid --no-header)
do
  if [ -f /proc/$pid/status ] && [ -f /proc/$pid/sched ];
  then
    ppid=$(awk '$1=="PPid:" {print $2}' /proc/$pid/status)

    sum_exec_runtime=$(awk '$1=="se.sum_exec_runtime" {print $3}' /proc/$pid/sched)

    nr_switches=$(awk '$1=="nr_switches" {print $3}' /proc/$pid/sched)

    art=$(echo $sum_exec_runtime $nr_switches | awk '{print $1/$2}')

    echo "ProcessID=$pid : Parent_ProcessID=$ppid : Average_Running_Time=$art"
  fi
done | sort -t "=" -k3 -n > four_out
