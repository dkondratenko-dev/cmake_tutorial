#!/bin/bash
echo "Building project..."
./build.sh

echo "Starting writer in background..."
# The writer will now exit on its own, so we don't need its PID
./build/writer &

# This sleep is still a good idea to ensure the writer has
# created the segment before the reader starts.
sleep 1

echo "Starting reader..."
./build/reader

echo "Cleaning up..."
# Use wait to allow all background jobs to finish cleanly.
# The writer will terminate by itself once the reader is done.
wait
echo "Done."