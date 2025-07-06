#!/bin/bash

proc=$(ps -u $USER -o pid,cmd --no-headers)
echo "$proc" | wc -l > first_out
echo "$proc" | awk -v $USER= '{print $1 " " $2}' >> first_out
