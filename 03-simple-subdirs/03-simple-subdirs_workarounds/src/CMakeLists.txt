# Collect all source files in src folder
set(SOURCES
    hello.cpp
)

# Issue: The variable SOURCES you set inside src/CMakeLists.txt is local to that directory 
# and not automatically propagated to the parent. ${SOURCES} will be empty at the top level.

# Workaround 3
# You can use CACHE variables to propagate data across CMake scopes, for example:
# set(SOURCES src/hello.cpp CACHE INTERNAL "Source files for src")

# But this is generally discouraged because:
# - It breaks the modular design of CMake targets.
# - Cache variables persist across CMake runs and may cause stale or unexpected behavior.
# - It makes maintenance and scaling more difficult.
