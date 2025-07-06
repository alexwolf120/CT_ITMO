#!/bin/bash

output=$(./6_the_fattest_one.bash)

mx_pid=$(echo "$output" | grep "Max Pid from /proc" | awk '{print $5}')

user=$(ps -o user= -p $mx_pid)

max_user=$(ps -eo user= | sort | uniq -c | sort -nr | head -n 1 | awk '{print $2}')

if [ "$user" == "$max_user" ]; then
    result="Пользователи совпадают."
else
    result="Пользователи не совпадают."
fi

echo "Пользователь сьевший много памяти $user" > result.txt
echo "Многопроцессный пользователь $max_user" >> result.txt
echo "$result" >> result.txt
