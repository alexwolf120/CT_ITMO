#!/usr/bin/bash

./4_inf.bash &
pid1=$!

./4_inf.bash &
pid2=$!

./4_inf.bash &
pid3=$!

cpulimit -p $pid1 -l 10 -b

sleep 10

sudo kill -9 $pid3
sudo kill -9 $pid1
sudo kill -9 $pid2
