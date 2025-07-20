#!/bin/bash

grep -E -R -h -o -s  "[a-zA-Z.+-]+@[a-zA-Z-]+\.[a-zA-Z.-]+" /etc | sort | uniq | awk '{printf "%s,", $1}' | sed 's/,$//' > etc_emails.lst
