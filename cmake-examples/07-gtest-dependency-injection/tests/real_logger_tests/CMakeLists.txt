# Define an executable target for the RealLogger tests
add_executable(RealLoggerTests real_logger_test.cpp)

# Link this test executable against:
# - InfrastructureLibrary: To get the RealLogger implementation.
# - gtest_main: Provides the main function to run Google Tests.
target_link_libraries(RealLoggerTests PRIVATE InfrastructureLibrary gtest_main)

# Add include directories so real_logger_test.cpp can find "infra/real_logger.h"
# ${CMAKE_SOURCE_DIR}/src ensures access to all source headers from the project root.
target_include_directories(RealLoggerTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add a test using CMake's CTest integration for this executable
add_test(NAME RealLoggerUnitTests COMMAND RealLoggerTests)
