cmake_minimum_required(VERSION 3.10)
project(MultiTargetExample)

add_executable(server server/src/server.cpp)
add_executable(client client/src/client.cpp)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Ctrl+/ - comment\uncomment

# Check if executable has debug symbol:
# file client 
# client: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=51bf1fb9bc4e09d8587338bf3195f06b50bd1799, for GNU/Linux 3.2.0, with debug_info, not stripped
