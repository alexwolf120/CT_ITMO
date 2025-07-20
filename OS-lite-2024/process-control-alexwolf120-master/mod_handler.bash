#!/bin/bash

random_number=$((1 + RANDOM % 20))
attempts=10

echo $$ > .handler_pid

signal() {
    case "$1" in
        USR1)
            input=$(cat .input_value)
            if [[ $input -eq $random_number ]]; then
                echo "Поздравляю! Вы угадали число $random_number."
                kill -SIGTERM $(cat .generator_pid)
                exit 0
            elif [[ $input -lt $random_number ]]; then
                echo "Загаданное число больше."
            else
                echo "Загаданное число меньше."
            fi
            ((attempts--))
            if [[ $attempts -le 0 ]]; then
                echo "Попытки закончились! Вы проиграли. Загаданное число было $random_number."
                kill -SIGTERM $(cat .generator_pid)
                exit 0
            fi
            ;;
        USR2)
            echo "Осталось попыток: $attempts"
            ;;
        SIGTERM)
            echo "Игра завершена. Спасибо за участие!"
            exit 0
            ;;
    esac
}

trap 'signal USR1' USR1
trap 'signal USR2' USR2
trap 'signal SIGTERM' SIGTERM

while true; do
    sleep 1
done
