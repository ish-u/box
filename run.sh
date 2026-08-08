#!/bin/bash

./app &
APP_PID=$!

cd web_server
uv run main.py </dev/null &
SERVER_PID=$!

wait $APP_PID

kill $SERVER_PID