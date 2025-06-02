cmake_minimum_required(VERSION 3.10)
project(MultiFileProject LANGUAGES CXX)

set(TARGET mathFunctions)

# Create executable target "main" from main.cpp initially
add_executable(${TARGET} main.cpp)

# Add more source files to "main" using target_sources
target_sources(${TARGET} PRIVATE math_functions.cpp)

# Include directory for headers (in this case, current directory)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR})