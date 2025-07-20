#!/bin/bash

width=$1
height=$2

x=$(( width / 2 ))
y=$(( height / 2 ))

print_position() {
    echo "x=$x;y=$y"
}

while true; do
    print_position
    read -n 1 -s keyb

    keyb=$(echo "$keyb" | tr '[:upper:]' '[:lower:]')

    case "$keyb" in
        w)
            ((y++))
            ;;
        a)
            ((x--))
            ;;
        s)
            ((y--))
            ;;
        d)
            ((x++))
            ;;
        q)
            break
            ;;
    esac

    if [ "$x" -lt 0 ] || [ "$y" -lt 0 ] || [ "$x" -ge "width" ] || [ "$y" -ge "$height" ]; then

        break
    fi
done
