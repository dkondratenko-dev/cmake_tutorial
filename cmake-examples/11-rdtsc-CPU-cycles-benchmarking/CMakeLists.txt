# Minimum CMake version
cmake_minimum_required(VERSION 3.16)

project(RDTSCBenchmarking CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(rdtsc_benchmarking main.cpp)
