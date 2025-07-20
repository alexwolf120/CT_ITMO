#!/bin/bash

./mod_handler.bash &
handler_pid=$!

./mod_producer.bash $handler_pid
