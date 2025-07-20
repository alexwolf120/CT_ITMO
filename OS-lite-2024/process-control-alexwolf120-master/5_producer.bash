#!/usr/bin/bash

echo $$ > .generator_pid

echo "Need a command please:"

while true; do
    read input
    echo "$input" > pipe
done

