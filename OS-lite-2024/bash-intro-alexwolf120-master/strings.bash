#!/bin/bash
while true; do
	read -r input

	if [ "$input" == "q" ]; then
		break
	fi

	len=${#input}
	if [[ "$input" =~ ^[a-zA-Z]+$ ]]; then
		bool=true
	else
		bool=false
	fi
	echo "$len"
	echo "$bool"
done
