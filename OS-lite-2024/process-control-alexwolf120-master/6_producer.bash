#!/bin/bash

echo $$ > .generator_pid

while true
do
    read -r input
    echo "Input $input"
    case "$input" in
        "+")
            kill -USR1 $(cat .handler_pid)
            ;;
        "*")
            kill -USR2 $(cat .handler_pid)
            ;;
        "TERM")
            kill -SIGTERM $(cat .handler_pid)
            break
            ;;
        *)
            :
            ;;
    esac
done
