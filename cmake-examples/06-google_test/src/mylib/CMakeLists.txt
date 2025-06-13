# Define a static library named 'mylib'
# The source files for this library are listed here
add_library(mylib STATIC calculator.cpp)

# Specify include directories for the library.
# The 'PRIVATE' keyword means these includes are only for building 'mylib' itself.
# 'PUBLIC' would mean consumers of this library would also get these includes.
target_include_directories(mylib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Mark the library as 'PUBLIC' or 'PRIVATE' for its dependencies.
# In this case, 'mylib' doesn't depend on other libraries directly,
# but if it did, you would link them here.