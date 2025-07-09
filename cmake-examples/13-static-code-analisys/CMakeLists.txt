cmake_minimum_required(VERSION 3.15)
project(FintechStaticAnalysisDemo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# G++ settings to enable all warnings
# Must have options enabled: -Wall -Wextra . At least because of a lot of undefined behaviour in C++
# -Wpedantic - should be checked from g++ docs ?
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wnull-dereference -Wold-style-cast -Wuseless-cast -Wnon-virtual-dtor")

# Enable clang-tidy for all targets if available
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
else()
    message(WARNING "clang-tidy not found: ${CLANG_TIDY_EXE}")    
    message(STATUS "Run to install>>\nsudo apt-get install clang-tidy")   
endif()

add_library(FinanceCalculator src/FinanceCalculator.cpp)
target_include_directories(FinanceCalculator PUBLIC include)

add_library(MistakesTest src/mistakes.cpp)
target_include_directories(MistakesTest PUBLIC include)

add_executable(finance_app src/main.cpp)
target_link_libraries(finance_app FinanceCalculator MistakesTest)

add_executable(test_calculator tests/test_calculator.cpp)
target_link_libraries(test_calculator FinanceCalculator)

# -------------------------------
# clang-tidy-all target
# -------------------------------
if(CLANG_TIDY_EXE)
    add_custom_target(clang-tidy-all
        COMMAND ${CLANG_TIDY_EXE}
            -p ${CMAKE_BINARY_DIR}
            -quiet
            $<TARGET_PROPERTY:FinanceCalculator,SOURCE_DIR>/src/FinanceCalculator.cpp
            $<TARGET_PROPERTY:finance_app,SOURCE_DIR>/src/main.cpp
            $<TARGET_PROPERTY:test_calculator,SOURCE_DIR>/tests/test_calculator.cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on all source files"
    )
endif()
