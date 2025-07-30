cmake_minimum_required(VERSION 3.16)
project(SynchronizedExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(producer producer.cpp)
add_executable(consumer consumer.cpp)

target_link_libraries(producer ${Boost_LIBRARIES})
target_link_libraries(consumer ${Boost_LIBRARIES})

if(WIN32)
    target_link_libraries(producer ws2_32 wsock32)
    target_link_libraries(consumer ws2_32 wsock32)
endif()
