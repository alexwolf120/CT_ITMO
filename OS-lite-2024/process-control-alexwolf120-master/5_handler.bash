#!/usr/bin/bash

MODE="add"
RESULT=1
gen_pid=$(cat .generator_pid)

echo "Result $RESULT"

tail -f pipe | while true; do
    read line
    case "$line" in
	"+")
        	MODE="add"
                echo "Add"
                ;;
        "*")
                MODE="multiply"
                echo "Multuply"
                ;;
        "QUIT")
                echo "Quit result $RESULT"
		kill -9 "$gen_pid"
                exit 0
                ;;
	*)
           	if [[ "$line" =~ ^-?[0-9]+$ ]]; then
                    NUMBER=$line
                    if [[ $MODE == "add" ]]; then
                        RESULT=$((RESULT + NUMBER))
                    elif [[ $MODE == "multiply" ]]; then
                        RESULT=$((RESULT * NUMBER))
                    fi
                    echo "Result $RESULT"
                else
                    echo "Incorrect '$line'" 
                        kill -9 "$gen_pid"
                    exit 1
                fi
                ;;
        esac
done

exit 0
