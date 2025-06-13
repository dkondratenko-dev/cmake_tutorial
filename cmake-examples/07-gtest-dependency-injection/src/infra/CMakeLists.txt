set(TARGET InfrastructureLibrary)

# Define a static library named 'InfrastructureLibrary'
# It contains the real_logger.cpp source file.
add_library(${TARGET} STATIC real_logger.cpp)

# Specify include directories for InfrastructureLibrary.
# PUBLIC makes these headers available to targets that link against InfrastructureLibrary.
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})