Cmake CTest:

1. # Add a test using CMake's CTest integration
# This allows you to run tests via `ctest` command or VS Code's test explorer
./tests/CMakeLists.txt:
add_test(NAME MyCalculatorTests COMMAND ${TARGET})

2. Add enable_testing() to ./CMakeLists.txt:
For CMake to generate the necessary CTestTestfile.cmake and other CTest-related files, you need to explicitly call enable_testing() in your main CMakeLists.txt. This command tells CMake that your project intends to use CTest for running tests.

Please add enable_testing() to your GTestProject/CMakeLists.txt file, preferably right after project(GTestProject LANGUAGES CXX).
enable_testing()

Execute:
cd ~/Documents/uw/Femi/cmake_tutorial-main/GTestProject/build
cmake ..
cmake --build .
ctest

# Add a custom target to run all tests

./CMakeLists.txt:
add_custom_target(run_unit_tests COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
                  DEPENDS calculator_ut # Ensure 'calculator_ut' executable is built before running
)

Execute:
cd ~/Documents/uw/Femi/cmake_tutorial-main/GTestProject/build
cmake ..
cmake --build . --target run_unit_tests

# List all targets

cmake --build . --target help