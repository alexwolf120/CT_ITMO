#!/bin/bash

ps aux | grep '/sbin/' | awk '{print $1}' > second_out
