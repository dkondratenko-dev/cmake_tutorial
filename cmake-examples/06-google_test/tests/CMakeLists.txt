set(TARGET calculator_ut)

# Add an executable target for your tests
# The source file for this executable is listed here
add_executable(${TARGET} calculator_test.cpp)

# Link your test executable against the necessary libraries
# 'mylib' is your own static library
# 'gtest_main' provides the main function for running Google Tests
# 'gmock_main' is for Google Mock, often used with GTest, but gtest_main implies it
target_link_libraries(${TARGET} PRIVATE mylib gtest_main)

# Add your source directory to the include path for the tests,
# so the test file can find "mylib/calculator.h"
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Add a test using CMake's CTest integration
# This allows you to run tests via `ctest` command or VS Code's test explorer
add_test(NAME MyCalculatorTests COMMAND ${TARGET})