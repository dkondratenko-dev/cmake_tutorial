# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(FintechPerfExample CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPORTANT: Set build type to Debug to include debug symbols (-g)
# This is VITAL for Valgrind to give you line numbers.
# We will build with `cmake .. -DCMAKE_BUILD_TYPE=Debug`
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Print the flags being used
message(STATUS "Compiler flags for ${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")


# --- Naive Target ---
add_executable(option_pricer_naive src/naive.cpp)
target_compile_features(option_pricer_naive PRIVATE cxx_std_17)

# --- Optimized Target ---
add_executable(option_pricer_optimized src/optimized.cpp)
target_compile_features(option_pricer_optimized PRIVATE cxx_std_17)