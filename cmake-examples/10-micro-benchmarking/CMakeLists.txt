# Minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Project name and language
project(ChronoBenchmarking CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add this definition to enable the benchmark code.
# For a release build, you can comment this line out to disable all
# benchmarking overhead.
add_compile_definitions(ENABLE_BENCHMARK)

# --- Benchmark Library ---
# Create a static library for our benchmarking tool
add_library(benchmark STATIC
    include/benchmark/Benchmark.cpp
)

# Make the 'include' directory available to any target that links this library
target_include_directories(benchmark PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# --- Main Executable ---
# Create the executable for our fintech application
add_executable(chrono_benchmark
    src/main.cpp
)

# Link our application against the benchmark library
target_link_libraries(chrono_benchmark PRIVATE benchmark)
