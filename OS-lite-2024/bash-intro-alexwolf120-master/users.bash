#!/bin/bash

awk -F: '{print $3, $1}' /etc/passwd | sort -k 1 -n
