#!/bin/bash

K=30
N=$1
k=30
count=0
while true
do
if [[ $count -ge $k ]]
then
	exit 0
fi
bash newmem.bash $N &
let count=$count+1
sleep 1
done
