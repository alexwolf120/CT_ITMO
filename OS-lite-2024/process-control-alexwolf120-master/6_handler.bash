#!/bin/bash

value=1

echo $$ > .handler_pid

signal() {
    case "$1" in
        USR1)
            ((value += 2))
            ;;
        USR2)
            ((value *= 2))
            ;;
        SIGTERM)
            echo "Stop"
            exit 0
            ;;
    esac
    echo "Value $value"
}

trap 'signal USR1' USR1
trap 'signal USR2' USR2
trap 'signal SIGTERM' SIGTERM

while true
do
    sleep 1
done
