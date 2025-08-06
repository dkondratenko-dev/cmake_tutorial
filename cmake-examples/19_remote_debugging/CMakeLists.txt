cmake_minimum_required(VERSION 3.10)
project(LongRunningCmake)

set(CMAKE_BUILD_TYPE Debug)

add_executable(long_running long_running.cpp)