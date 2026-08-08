#!/bin/bash

uv --directory web_server run main.py &
SERVER_PID=$!

GPIO_PID=""

if [[ "$(uname)" != "Darwin" ]]; then
    python3 gpio/main.py &
    GPIO_PID=$!
fi

sleep 5

./app

if [[ -n "$GPIO_PID" ]]; then
    kill "$GPIO_PID" 2>/dev/null || true
    wait "$GPIO_PID" 2>/dev/null || true
fi

kill "$SERVER_PID" 2>/dev/null || true
wait "$SERVER_PID" 2>/dev/null || true