#!/bin/bash

./6_handler.bash &
handler_pid=$!  

./6_producer.bash $handler_pid

