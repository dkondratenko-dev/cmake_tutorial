add_library(mylib DataProcessor.cpp DataProcessor.h)
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
