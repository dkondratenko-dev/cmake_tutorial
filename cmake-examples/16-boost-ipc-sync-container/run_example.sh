#!/bin/bash
#The .sh extension means "SHell" script
echo "Building project..."
./build.sh

#Start producer in background. On line 8, $! retrieves the PID of the most recently started background process and assigns it to the variable PRODUCER_PID
echo "Starting producer in background..."
./build/producer &
PRODUCER_PID=$!

sleep 0.5

#Start consumer in background and set a pid for it
echo "Starting consumer..."
./build/consumer &
CONSUMER_PID=$!

echo "Both processes running. Producer will finish automatically."
echo "Press Ctrl+C to stop early, or wait for completion..."

# Wait for consumer to finish
wait $CONSUMER_PID

# Cleanup producer if still running
kill $PRODUCER_PID 2>/dev/null
wait $PRODUCER_PID 2>/dev/null

echo "Example completed."
