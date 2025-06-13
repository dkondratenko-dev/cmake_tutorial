set(TARGET my_application)

add_subdirectory(app)   # Application logic
add_subdirectory(infra) # Infrastructure (interfaces, real implementations)

# Define the main application executable
add_executable(${TARGET} main.cpp)

# Link the main application to your application logic library (ApplicationLibrary)
# and your infrastructure library (InfrastructureLibrary) where the logger is defined.
target_link_libraries(${TARGET} PRIVATE ApplicationLibrary InfrastructureLibrary)

# Add include directories so main.cpp can find headers like "app/user_processor.h"
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

