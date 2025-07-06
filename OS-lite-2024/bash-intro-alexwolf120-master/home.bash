#!/bin/bash

current_dir=$(pwd)

home_dir=$HOME

[ "$current_dir" = "$home_dir" ] && echo "$home_dir" && exit 0

echo "not home dir" && exit 1
