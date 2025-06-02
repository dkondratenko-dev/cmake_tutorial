# Create a library target from hello.cpp
add_library(hello hello.cpp)

# Let the library know where to find headers
target_include_directories(hello PUBLIC ${PROJECT_SOURCE_DIR}/include)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
