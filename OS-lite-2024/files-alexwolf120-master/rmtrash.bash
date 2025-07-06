#!/bin/bash

if [[ $# -ne 1 ]]
then
	echo "incorrect arguments (must be only file name)"
	exit 1
fi

if [[ ! -f $1 ]]
then
	echo "file $1 don't exits"
	exit 2
fi

if [[ ! -d ~/.trash ]]
then
	mkdir ~/.trash
	touch ~/.trash/num1
fi

if [[ ! -f ~/.trash.log ]]
then
	touch ~/.trash.log
fi

count=1
while [[ ! -f ~/.trash/num$count ]]
do
let count=$count+1
done

ln "$1" ~/.trash/$count
rm -f "$1"
echo "$PWD/$1 -> ~/.trash/$count" >> ~/.trash.log

rm ~/.trash/num$count
let count=$count+1
touch ~/.trash/num$count
