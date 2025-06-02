# 📘 CMake Tutorial — Multi-file Projects with `target_sources`

This example shows how to add multiple `.cpp` files to a single executable using `target_sources`.  
We separate mathematical functions into their own source file.

## Explanation

**add_executable(main main.cpp)** creates the executable target initially with just main.cpp.

**target_sources(main PRIVATE math_functions.cpp)** adds math_functions.cpp to the sources of the main target.

**target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR})** tells the compiler where to find header files.

This approach is more flexible than listing all sources inside add_executable() because it allows adding source files in multiple steps or from different CMakeLists files (useful for bigger projects).

Compared to the previous example where all sources were defined upfront in a variable or directly in add_executable(), here you can organize sources more modularly, e.g., adding files as the project grows.

Use this approach when your project starts with a few files but you expect to add more source files later or want to keep your CMakeLists organized and maintainable.
