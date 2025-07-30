# Boost.Asio Async Networking Example

This example demonstrates asynchronous TCP communication between processes that can run on different machines.

## Building
```bash
./build.sh
```

## Running on Different Machines

### On Server Machine:
```bash
./build/server
```
The server will listen on port 12345.

### On Client Machine:
1. Edit client.cpp and change the host variable to the server's IP address:
   ```cpp
   std::string host = "192.168.1.100";  // Replace with actual server IP
   ```
2. Rebuild: `./build.sh`
3. Run: `./build/client`

## Running Locally (Same Machine):
```bash
./run_local_example.sh
```

## Firewall Notes:
- Ensure port 12345 is open on the server machine
- For production use, consider using different ports and proper security measures

## What This Example Shows:
- Asynchronous TCP server that can handle multiple clients
- Asynchronous TCP client that connects and sends messages
- Non-blocking I/O operations using Boost.Asio
- Echo server pattern (server responds to each message)
