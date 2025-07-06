#!/bin/bash

if [[ ! -d ~/restore ]]
then
	mkdir ~/restore
fi

last_backup=$(ls ~ | grep '^Backup-[0-9]\{4\}-[0-1][0-9]-[0-3][0-9]$' | sort | tail -n 1)

if [[ -z $last_backup ]]
then
	echo "no backup files"
	exit 0
fi

find ~/$last_backup -type f | while read -r file
do
	if [[ $file =~ ^.*\.[0-9]{4}-[0-1][0-9]-[0-3][0-9]$ ]]
	then
		continue
	else
		file_name=$(basename "$file")
		cp $file ~/restore/$file_name
	fi
done
