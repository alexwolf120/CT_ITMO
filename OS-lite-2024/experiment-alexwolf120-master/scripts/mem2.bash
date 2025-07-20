#!/bin/bash

rm -f report2.log

arr=()
count=0
while true
do
	let count=$count+1
	let d=$count%100000
	if [[ 0 -ne $count ]] && [[ $d -eq 0 ]]
	then
		echo $count >> report2.log
	fi
	array+=(9 8 7 6 5 4 3 2 1 0)
done
