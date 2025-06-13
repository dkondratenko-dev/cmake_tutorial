# Add subdirectories for your tests
add_subdirectory(my_app_test)
add_subdirectory(real_logger_tests) # NEW: Add the new RealLogger test subdirectory

# Add a custom target to run all tests (convenience target)
# IMPORTANT: Updated DEPENDS to include both test executables
add_custom_target(run_tests COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
                  DEPENDS MyAppTests RealLoggerTests # Ensure both test executables are built first
)
