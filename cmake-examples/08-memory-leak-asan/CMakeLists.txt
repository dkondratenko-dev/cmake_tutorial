# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(MemoryLeakASAN CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Prefer standard-compliant code

# --- Set default CMAKE_BUILD_TYPE if not already specified by the user ---
# This line checks if CMAKE_BUILD_TYPE is NOT DEFINED.
# If it's not defined (meaning the user didn't set it via -DCMAKE_BUILD_TYPE=...),
# then it sets it to "Debug" and stores it in the cache.
# The CACHE keyword makes it persistent for subsequent CMake runs.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, e.g. Debug, Release, RelWithDebInfo, MinSizeRel.")
    # The 'STRING' argument tells CMake that this is a string variable.
    # The last argument is a help string for the user in GUI tools like cmake-gui.
endif()
# --- End of default CMAKE_BUILD_TYPE setting ---

# --- How to connect AddressSanitizer (ASan) ---
# ASan adds instrumentation to your code at compile-time and runtime.
# It works best with debug information, so it's usually enabled for Debug or RelWithDebInfo builds.
# It introduces some performance overhead, so it's typically *not* used for Release builds.

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "Enabling AddressSanitizer for build type: ${CMAKE_BUILD_TYPE}")

    # Compiler flags for ASan:
    # -fsanitize=address: Enables ASan for memory error detection.
    # -fno-omit-frame-pointer: Helps ASan generate more accurate stack traces.
    # -g: Includes debug symbols, crucial for ASan to show file names and line numbers.
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)

    # Linker flags for ASan:
    # -fsanitize=address: Links the ASan runtime library.
    add_link_options(-fsanitize=address)
else()
    message(STATUS "AddressSanitizer not enabled for build type: ${CMAKE_BUILD_TYPE}")
endif()

# Define your executable target and list all source files
add_executable(memory-leak-asan
    src/main.cpp
    src/Trade.cpp
    src/TradeProcessor.cpp
)

# Specify the include directories so the compiler can find your .hpp files
target_include_directories(memory-leak-asan
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# If you had other libraries (e.g., GTest), you would link them here:
# target_link_libraries(FintechAsanExample PRIVATE YourOtherLibrary)