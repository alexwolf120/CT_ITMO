#!/bin/bash

echo $$ > .generator_pid

handler_pid=$1

while true; do
    echo -n "Введите число, SHOW или QUIT: "
    read input
    case "$input" in
        [0-9]*)
            echo "$input" > .input_value
            kill -USR1 $handler_pid
	    sleep 1
            ;;
        "SHOW")
            kill -USR2 $handler_pid
	    sleep 1
            ;;
        "QUIT")
            kill -SIGTERM $handler_pid
            break
            ;;
        *)
            echo "Некорректный ввод. Попробуйте снова."
            ;;
    esac
done
