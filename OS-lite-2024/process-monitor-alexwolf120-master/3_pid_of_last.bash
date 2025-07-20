#!/bin/bash

ps au --sort=start_time --no-header | tail -n -1 | awk '{print $2 " " $9}'
