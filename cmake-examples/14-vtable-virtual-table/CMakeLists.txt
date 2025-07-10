# =============================================================================
# CMake Project Configuration
# =============================================================================

# Specify the minimum version of CMake required to build this project.
cmake_minimum_required(VERSION 3.16)

# Define the project name and programming languages used.
project(VTable_Examples LANGUAGES CXX)

# =============================================================================
# Global Project Settings
# =============================================================================

# Set the C++ standard for the entire project.
# We use C++17 as a modern, widely-supported standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions for better portability.

# =============================================================================
# Target Definitions
# =============================================================================
# In this section, we define each of the three executables as separate targets.

# --- Target 1: hello_vtable ---
# A simple example contained in a single file.
add_executable(
    hello_vtable
    src/hello_vtable/main.cpp
)

# --- Target 2: fintech_vtable ---
# A more complex example with multiple source files in subdirectories.
# We must list all .cpp files that need to be compiled and linked together.
add_executable(
    fintech_vtable
    src/fintech_vtable/main.cpp
    src/fintech_vtable/stock/Stock.cpp
    src/fintech_vtable/bond/Bond.cpp
)

# --- Target 3: diamond_problem ---
# An example demonstrating the diamond problem and virtual inheritance.
add_executable(
    diamond_problem
    src/diamond_problem/main.cpp
)

# =============================================================================
# Include Directories and Compile Options
# =============================================================================
# Here we configure include paths and compiler warnings for all targets.

# Create a list of all targets to apply common settings.
set(ALL_TARGETS
    hello_vtable
    fintech_vtable
    diamond_problem
)

# Use a loop to apply settings to each target. This is cleaner than repeating
# commands for each executable.
foreach(target IN LISTS ALL_TARGETS)
    # --- Include Directories ---
    # Add the top-level 'src' directory to the include path for each target.
    # This allows us to use includes like: #include "fintech_vtable/TradableAsset.h"
    # from anywhere within that target's source files. This creates unambiguous and
    # location-independent include paths.
    target_include_directories(${target}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # --- Compiler Warnings ---
    # Enabling a high level of warnings is a best practice for writing robust code.
    if(MSVC)
        # Options for Microsoft Visual C++
        target_compile_options(${target} PRIVATE /W4 /WX) # /W4 is a high warning level, /WX treats warnings as errors.
    else()
        # Options for GCC and Clang
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror # Treat all warnings as errors
        )
    endif()
endforeach()

# =============================================================================
# Installation (Optional)
# =============================================================================
# Defines where the executables will be placed when 'make install' is run.
install(TARGETS ${ALL_TARGETS}
    RUNTIME DESTINATION bin
)

# A message to the user upon successful configuration.
message(STATUS "Project configured successfully. You can now build the targets: ${ALL_TARGETS}")