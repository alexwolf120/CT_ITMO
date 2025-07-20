#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <group>"
  exit 1
fi

GROUP=$1

FILE="students.txt"

if [ ! -f "$FILE" ]; then
  echo "File $FILE not found!"
  exit 1
fi

grep "^$GROUP," "$FILE" | sort -t',' -k3nr | head -n3 | awk -F',' '{print $2}' | awk -F' ' '{print $1, $2}'
