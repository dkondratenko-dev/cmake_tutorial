cmake_minimum_required(VERSION 3.16)
project(AsioExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(server server.cpp)
add_executable(client client.cpp)

target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(client ${Boost_LIBRARIES})

if(WIN32)
    target_link_libraries(server ws2_32 wsock32)
    target_link_libraries(client ws2_32 wsock32)
else()
    target_link_libraries(server pthread)
    target_link_libraries(client pthread)
endif()
