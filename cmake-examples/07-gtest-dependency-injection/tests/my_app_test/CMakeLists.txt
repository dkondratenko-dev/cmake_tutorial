# Add an executable target for your tests
# The source file for this executable is my_app_test.cpp.
add_executable(MyAppTests my_app_test.cpp)

# Link your test executable against the necessary libraries:
# - ApplicationLibrary: Your application logic (UserProcessor).
# - InfrastructureLibrary: Your infrastructure (ILogger interface, RealLogger if needed for non-mocked tests).
# - gmock_main: Provides the main function for Google Mock tests, and includes gtest_main.
target_link_libraries(MyAppTests PRIVATE ApplicationLibrary InfrastructureLibrary gmock_main)

# Add include directories so the test file can find headers like "app/user_processor.h"
# and "infra/ilogger.h".
# The ${CMAKE_SOURCE_DIR}/src path ensures access to all source headers.
target_include_directories(MyAppTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add a test using CMake's CTest integration
# This allows you to run tests via `ctest` command or VS Code's test explorer.
add_test(NAME UserProcessorTests COMMAND MyAppTests)