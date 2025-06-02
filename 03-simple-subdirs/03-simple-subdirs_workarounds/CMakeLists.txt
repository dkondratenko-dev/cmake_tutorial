cmake_minimum_required(VERSION 3.10)

project(SimpleSubdirs LANGUAGES CXX)

# Add the src folder to process its CMakeLists.txt
add_subdirectory(src)

# Include headers globally
include_directories(include)

# Create executable with main.cpp and all src files
add_executable(main main.cpp ${SOURCES})


# Workaround 1
# set(SOURCES src/hello.cpp)

# Workaround 2
# Automatically find all .cpp files under src/ recursively.
# Pros:
    # Easy for small projects or quick prototyping.
    # No need to maintain lists of sources manually.
# Cons:
    # CMake recommends not using GLOB to track source files for large projects, because:
    # Adding/removing files won’t trigger re-run of CMake unless you re-run manually (CONFIGURE_DEPENDS mitigates this partially).
    # Harder to explicitly control sources and dependencies.
    # You lose granularity (e.g., per-library or per-module source grouping).
    # Less clear structure, harder to modularize.

# file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# message("Simple: Sources: ${SOURCES}")

# if(NOT SOURCES)
#     # Diagnostic message (shown always)
#     message(STATUS "Status: SOURCES variable is empty!")

#     # Debug message (shown only if CMake is run with --log-level=DEBUG)
#     message(DEBUG "Debug: SOURCES variable is empty — check your source file paths.")

#     # Or you can use WARNING to show it more prominently without stopping
#     message(WARNING "Warning: SOURCES variable is empty!")

#     message(FATAL_ERROR "Error: SOURCES variable is empty! No source files found.")
# else()
#     message(STATUS "Status: Sources: ${SOURCES}")
# endif()



