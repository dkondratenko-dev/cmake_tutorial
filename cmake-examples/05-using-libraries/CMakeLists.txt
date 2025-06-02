cmake_minimum_required(VERSION 3.10)
project(StructuredHello LANGUAGES CXX)

# Add subdirectory with source files and CMakeLists.txt
add_subdirectory(src)

# Include directory with headers
include_directories(include)

# Create main executable
add_executable(main main.cpp)

# Link the hello library to the main executable
target_link_libraries(main PRIVATE hello)
