#!/bin/bash

man bash | grep -o '[[:alpha:]]\{4,\}' |sort | uniq -c | sort -nr | awk '{print $2}' | head -n 3
