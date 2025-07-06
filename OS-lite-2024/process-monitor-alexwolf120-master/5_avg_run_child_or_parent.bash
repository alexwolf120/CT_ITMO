#!/bin/bash

input_file="four_out"
output_file="five_out"

./4_cpu_burst.bash

s_ppid=""
total_art=0
count=0

rm -f "$output_file"
touch "$output_file"

while read line; do
    ppid=$(echo $line | cut -d '=' -f 3 | awk '{print $1}')
    art=$(echo $line | cut -d '=' -f 4 | awk '{print $1}')

    if [[ -z $s_ppid ]]; then
        s_ppid="$ppid"
    fi

    if [[ "$ppid" -eq "$s_ppid" ]]; then
        total_art=$(echo $total_art $art | awk '{print $1 + $2}')
        count=$(($count + 1))
    else
        if [[ "$count" -gt 0 ]]; then
            average_art=$(echo $total_art $count | awk '{print $1 / $2}')
            echo "Average_Running_Children_of_ParentID=$s_ppid is $average_art" >> $output_file
        fi
        s_ppid="$ppid"
        total_art="$art"
        count=1
    fi

    echo $line >> $output_file
done < $input_file

if [[ "$count" -gt 0 ]]; then
    average_art=$(echo $total_art $count | awk '{print $1 / $2}')
    echo "Average_Running_Children_of_ParentID=$s_ppid is $average_art" >> $output_file
fi
