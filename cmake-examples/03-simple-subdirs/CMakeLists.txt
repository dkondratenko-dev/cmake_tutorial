cmake_minimum_required(VERSION 3.10)

project(SimpleSubdirs LANGUAGES CXX)

# Add the src folder to process its CMakeLists.txt
add_subdirectory(src)

# Include headers globally
include_directories(include)

# Create executable with main.cpp and all src files
add_executable(main main.cpp ${SOURCES})
