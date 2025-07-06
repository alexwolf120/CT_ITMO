#!/bin/bash

next_name() {
	file_name=$1
	file_path=$2
	file_name_begin=${file_name%.*}
        file_name_end=${file_name##*.}
        if [[ $file_name == $file_name_end ]] 
	then
	        file_name_end=""
        else
                file_name_end=".$file_name_end"
        fi
        file_num=1
        while [[ -e "$file_path/$file_name_begin($file_num)$file_name_end" ]]
        do
                let file_num=$file_num+1
        done
	echo "$file_name_begin($file_num)$file_name_end"
}

flag="i"

if [[ $# -eq 2 ]]
then
	case $1 in
		-i|--ignore)
			flag="i"
			;;
		-u|--unique)
			flag="u"
			;;
		-o|--overwrite)
			flag="o"
			;;
		*)
			echo "you can use one of these option only:"
			echo "-i, --ignore      do nothing if file exists"
			echo "-u, --unique      add unique number after filename"
			echo "-o, --overwrite   overwrite existing file"
			exit 1
			;;
	esac
	untrash_file_name=$2
elif [[ $# -eq 1 ]]
then
	untrash_file_name=$1
else
	echo "incorrect arguments, must be [[option]] and filename only"
	exit 2
fi

count=0

while read -r line
do
	full_file_name=$(echo "$line" | awk -F ' -> ' '{print $1}')
	file_name=$(basename "$full_file_name")
	file_path=$(dirname "$full_file_name")
	new_name=$HOME$(echo $line | awk -F ' -> ' '{print $2}' | sed 's/~//1')
	if [[ "$file_name" == "$untrash_file_name" ]] && [[ -f $new_name ]]
	then
		let count=$count+1
		echo "would you like choose $full_file_name?"
		answer=idk
		while [[ $answer != yes ]] && [[ $answer != no ]]
		do
			read answer<&1
		done
		if [[ $answer == yes ]]
		then
			if [[ ! -d $file_path ]]
			then
				echo "file will be in home directory"
				file_path=$HOME
			fi

			if [[ -e $file_path/$file_name ]]
			then
				case $flag in
					"i")
						echo "this file already exists"
						continue
					;;
					"o")
						rm -f $file_path/$file_name
					;;
					"u")
						file_name=$(next_name $file_name $file_path)
					;;
				esac
			fi
			ln $new_name "$file_path/$file_name"
			rm -f $new_name
			exit 0
		fi
	fi
done <<< $(cat ~/.trash.log)

if [[ count -eq 0 ]]
then
	echo "no information about file with name $untrash_file_name"
	exit 0
fi
echo "no more variants"
