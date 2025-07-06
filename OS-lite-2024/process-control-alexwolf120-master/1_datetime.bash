#!/bin/bash

current_datetime=$(date +"%Y-%m-%d_%H-%M-%S")
current_date=$(date +"%Y-%m-%d")

mkdir -p ~/test

touch ~/test/"$current_datetime"

echo "<$current_datetime> test was created successfully" >> ~/report

mkdir -p ~/test/archived

for file in ~/test/*; do
    if [[ -f "$file" && "$(basename "$file")" != "$current_datetime" ]]; then
        file_date=$(basename "$file" | cut -d'_' -f1)
        if [ "$file_date" != "$current_date" ]; then
            tar -czf ~/test/archived/"$file_date".tar.gz -C ~/test "$file_date"*
            rm ~/test/"$file_date"*
        fi
    fi
done
