#!/bin/bash

mkfifo pipe

bash 5_producer.bash & bash 5_handler.bash

