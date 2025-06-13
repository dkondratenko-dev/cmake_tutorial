# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.16)

# Define your project name and C++ language
project(GTestMockDIProject LANGUAGES CXX)

# Enable testing for CTest integration
enable_testing()

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable fetching Google Test and Google Mock as sub-modules
# This downloads their source code into your build directory.
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1 # Specify a stable release tag for GTest/GMock
)
FetchContent_MakeAvailable(googletest)

# Add subdirectories for your source code
add_subdirectory(src)       # Contains main.cpp for the executable

# Add subdirectories for your tests
add_subdirectory(tests)

