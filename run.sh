#!/bin/bash

uv --directory web_server run main.py &
SERVER_PID=$!

./app

kill "$SERVER_PID" 2>/dev/null
wait "$SERVER_PID" 2>/dev/null