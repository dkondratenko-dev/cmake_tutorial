#!/bin/bash
echo "Building project..."
./build.sh

echo "Starting writer in background..."
./build/writer &
WRITER_PID=$!

sleep 1

echo "Starting reader..."
./build/reader

echo "Cleaning up..."
kill $WRITER_PID 2>/dev/null
wait $WRITER_PID 2>/dev/null
