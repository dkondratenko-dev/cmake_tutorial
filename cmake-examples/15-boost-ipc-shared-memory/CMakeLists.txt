cmake_minimum_required(VERSION 3.16)
project(SharedMemoryExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(writer writer.cpp)
add_executable(reader reader.cpp)

target_link_libraries(writer ${Boost_LIBRARIES})
target_link_libraries(reader ${Boost_LIBRARIES})

if(WIN32)
    target_link_libraries(writer ws2_32 wsock32)
    target_link_libraries(reader ws2_32 wsock32)
endif()
