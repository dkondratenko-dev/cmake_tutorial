# 📘 CMake Tutorial — Structured Project with `include_directories` and `add_subdirectory`

This tutorial shows how to organize a simple C++ project using CMake with a modular folder structure:

- Source files in `src/`
- Header files in `include/`
- Main executable built from all sources

---

## Project Structure

03-simple-subdirs/
├── CMakeLists.txt
├── include/
│   └── hello.h
├── src/
│   ├── CMakeLists.txt
│   └── hello.cpp
└── main.cpp

---

## Root `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.10)
project(SimpleSubdirs LANGUAGES CXX)

# Add the src directory, which contains source files list
add_subdirectory(src)

# Add the include directory for header files globally
include_directories(include)

# Create executable target "main" from main.cpp and sources from src/
add_executable(main main.cpp ${SOURCES})
````

---

## `src/CMakeLists.txt`

```cmake
# Define the SOURCES variable with all source files in src/
set(SOURCES
    hello.cpp
)
```

---

## `include/hello.h`

```cpp
#ifndef __HELLO_H__
#define __HELLO_H__

void say_hello();

#endif // __HELLO_H__
```

---

## `src/hello.cpp`

```cpp
#include <iostream>
#include "hello.h"

void say_hello() {
    std::cout << "Hello from src!" << std::endl;
}
```

---

## `main.cpp`

```cpp
#include "hello.h"

int main() {
    say_hello();
    return 0;
}
```

---

## How to Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
./main
```

---

## Expected Output

```bash
Hello from src!
```

---

## Explanation

- `add_subdirectory(src)` includes the `src` folder's CMake file to add source files.
- `include_directories(include)` makes headers in `include/` available for all targets.
- The `SOURCES` variable from `src/CMakeLists.txt` is used in the root `add_executable` command to compile all source files together.
- This setup keeps your project modular and organized without using libraries.

---

Feel free to expand this structure with libraries and more complex modules in future lessons!
