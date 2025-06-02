# Trainer Notes — 01: Basic Hello

## cmake_minimum_required(VERSION 3.10)

cmake_minimum_required(VERSION x.y)
This command ensures that the CMake version being used supports all the commands and features in your CMakeLists.txt.

It's important when your project runs on another machine — if the user's CMake version is older than required, CMake will show an error immediately, letting the user know they need to update it.

```cmake
  cmake_minimum_required(VERSION <major>[.<minor>[.<patch>]] [FATAL_ERROR])
```

We use version 3.10 because it is common and stable.

Some features need newer versions, we will see them later.

```cmake
project(BasicHello VERSION 1.0 LANGUAGES CXX)
```

💡 Note: You can omit LANGUAGES and CMake will default to **C and CXX**.

Declares a CMake project.

BasicHello is the project name.

VERSION 1.0 is optional, but a good habit.

LANGUAGES CXX tells CMake this is a C++ project.
Other supported options: C, CXX, CUDA, Fortran, ASM, OBJC, OBJCXX, HIP, ISPC, Swift
The full list can be found here: <https://cmake.org/cmake/help/latest/command/project.html>

Syntax:

```cmake
project(<name> [VERSION <major>[.<minor>[.<patch>]]] [LANGUAGES <lang>...])
add_executable(hello src/main.cpp)
```

Adds a target that will be compiled to an executable.
'hello' is the name of the executable.

src/main.cpp is the source file.

Syntax:

```cmake
add_executable(<name> [WIN32|MACOSX_BUNDLE] [EXCLUDE_FROM_ALL] source1 [source2 ...])
```

## Generators list

cmake  - - help – will print generators available in the system
cmake -G “Visual Studio 16 2019” ..

## 🔧 Build Instructions

### 1. Create a separate build folder

This keeps the main project folder clean. All build files and system-specific scripts will go here.  
It’s common to add this folder to `.gitignore`.

```bash
mkdir build
```

### 2. Enter the build directory

```bash
cd build
```

### 3. Generate build files using CMake

This step reads the `CMakeLists.txt` from the parent folder:

```bash
cmake ..
```

### 4. Build the project

Use the following command to build the target (e.g., `helloWorld`) in **Release** mode:

```bash
cmake --build . --config Release --target helloWorld
```

---

✅ **Tip**:  

- On **Linux**, `--config Release` is usually optional.  
- On **Windows** (e.g., with Visual Studio), `--config` is often required.

## Notes

It’s a good practice to keep source files inside a src/ directory.

Later examples will include:

include/ folder for headers

linking external libraries

compile options via target_compile_features and more
