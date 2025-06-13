# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.16)

# Define your project name and language
project(GTestProject LANGUAGES CXX)

# Set the C++ standard to C++17 (or your preferred standard)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

# Enable fetching Google Test (GTest) as a sub-module
# This is a common way to include GTest without installing it system-wide
# It downloads GTest source code into your build directory.
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1 # Specify a stable release tag
)
FetchContent_MakeAvailable(googletest)

# Add your source code subdirectory
add_subdirectory(src/mylib)

# Add your test subdirectory
add_subdirectory(tests)


# Add a custom target to run all tests
# This target depends on all test executables to ensure they are built before tests are run.
# The `ALL` keyword would make it part of the default build, but usually you want to
# run tests explicitly, so we omit ALL.
add_custom_target(run_unit_tests COMMAND ${CMAKE_CTEST_COMMAND} --extra-verbose --output-on-failure --stop-on-failure
                  DEPENDS calculator_ut # Ensure 'calculator_ut' executable is built before running
)