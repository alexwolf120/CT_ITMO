#!/bin/bash

if [[ ! -f ~/backup-report ]]
then
	touch ~/backup-report
fi

date=$(date +"%Y-%m-%d")
now=Backup-$date
last_week=Backup-$(date --date='7 days ago' +"%Y-%m-%d")
last_backup=$(ls ~ | grep '^Backup-[0-9]\{4\}-[0-1][0-9]-[0-3][0-9]$' | sort | tail -n 1)
if [[ -z $last_backup ]] || [[ $last_week > $last_backup ]]
then
	mkdir ~/$now
	cp ~/source/* ~/$now
	echo new directory ~/$now created at $(date +"%Y-%m-%d")$'\n'"copied files:"$'\n'$(ls ~/source) >> ~/backup-report
else
	echo "update in directory" ~/$last_backup at $date$'\n'"new copied files:" >> ~/backup-report
	for file in $(ls ~/source)
	do
		if [[ ! -f ~/$last_backup/$file ]]
		then
			cp ~/source/$file ~/$last_backup
			s1="$s1$file "
		else
			if [[ $(du ~/source/$file | awk '{print $1}') -ne $(du ~/$last_backup/$file | awk '{print $1}') ]]
			then
				mv ~/$last_backup/$file ~/$last_backup/$file.$date
				cp ~/source/$file ~/$last_backup
				s2="$s2$file -> $file."$date" "
			fi
		fi
	done
	echo $s1$'\n'"changed files:"$'\n'$s2 >> ~/backup-report
fi
