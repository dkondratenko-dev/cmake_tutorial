FROM ubuntu:22.04

# Install necessary tools
RUN apt-get update && apt-get install -y \
    g++ \
    gdb \
    gdbserver \
    make

# Copy your source code
WORKDIR /app
COPY . .

# Compile the program with debug symbols
RUN g++ -g -o main main.cpp

# Keep container running
CMD ["tail", "-f", "/dev/null"]