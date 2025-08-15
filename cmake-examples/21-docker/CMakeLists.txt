cmake_minimum_required(VERSION 3.16)
project(DockerDebugDemo LANGUAGES CXX)

# Set the default build type to Debug if it's not already defined.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src/mylib)
add_subdirectory(app)
add_subdirectory(tests)
