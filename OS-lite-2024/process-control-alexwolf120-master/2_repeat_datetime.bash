#!/bin/bash

(sleep 10; ./1_datetime.bash) &

tail -n 0 -f ~/report &
