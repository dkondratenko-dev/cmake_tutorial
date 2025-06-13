set(TARGET ApplicationLibrary)
# Define a static library named 'ApplicationLibrary'
# It contains the user_processor.cpp source file.
add_library(${TARGET} STATIC user_processor.cpp)

# Link ApplicationLibrary to InfrastructureLibrary, as UserProcessor depends on ILogger (from InfrastructureLibrary).
target_link_libraries(${TARGET} PUBLIC InfrastructureLibrary)

# Specify include directories for ApplicationLibrary.
# PRIVATE makes this include path internal to ApplicationLibrary's compilation.
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
