#!/bin/bash
#The .sh extension means "SHell" script
echo "Building project..."
./build.sh

echo "Starting server in background..."
./build/server &
SERVER_PID=$!

sleep 1

echo "Starting client..."
./build/client

echo "Cleaning up..."
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null
