#!/bin/bash
set -e

cd "$(dirname "$0")"

git pull
uv --directory web_server sync
make

uv --directory web_server run main.py &
SERVER_PID=$!

soloist -n "box" -k "$SOLOIST_API_KEY" -w "127.0.0.1:5010" &
SOLOIST_PID=$!

GPIO_PID=""

if [[ "$(uname)" != "Darwin" ]]; then
    python3 gpio/main.py &
    GPIO_PID=$!
fi

./app


if [[ -n "$SOLOIST_PID" ]]; then
    kill "$SOLOIST_PID" 2>/dev/null || true
    wait "$SOLOIST_PID" 2>/dev/null || true
fi

if [[ -n "$GPIO_PID" ]]; then
    kill "$GPIO_PID" 2>/dev/null || true
    wait "$GPIO_PID" 2>/dev/null || true
fi

kill "$SERVER_PID" 2>/dev/null || true
wait "$SERVER_PID" 2>/dev/null || true