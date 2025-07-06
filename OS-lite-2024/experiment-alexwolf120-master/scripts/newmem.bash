#!/bin/bash

if [[ $# -ne 1 ]]
then
exit 1
fi

rm -f report.log

arr=()
count=0
while true
do
	if [[ $count -ge $1 ]]
	then
		exit 0
	fi
	let count=$count+1
	let d=$count%100000
	if [[ 0 -ne $count ]] && [[ $d -eq 0 ]]
	then
		echo $count >> report.log
	fi
	array+=(9 8 7 6 5 4 3 2 1 0)
done
