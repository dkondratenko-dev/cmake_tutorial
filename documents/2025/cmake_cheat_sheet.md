- [A Developer's Guide to CMake Build Commands](#a-developers-guide-to-cmake-build-commands)
  - [1. Basic CMake Workflow](#1-basic-cmake-workflow)
    - [1.1. Configuration: Generating Build Files](#11-configuration-generating-build-files)
    - [1.2. Building: Compiling Your Project](#12-building-compiling-your-project)
  - [2. Controlling Build Type (Debug, Release)](#2-controlling-build-type-debug-release)
  - [3. Listing Targets](#3-listing-targets)
  - [4. Building Specific Targets](#4-building-specific-targets)
  - [5. Enabling AddressSanitizer (ASan) for Debugging](#5-enabling-addresssanitizer-asan-for-debugging)
  - [6. Cleaning the Build Directory](#6-cleaning-the-build-directory)
  - [7. Advanced Configuration Options](#7-advanced-configuration-options)
  - [Conclusion](#conclusion)

# A Developer's Guide to CMake Build Commands

CMake is a powerful, cross-platform build system generator. It doesn't build your project directly; instead, it generates native build tool files (like Makefiles or Visual Studio projects) which are then used to compile your code. This guide covers fundamental CMake commands for configuring, building, and debugging your C++ projects.

## 1. Basic CMake Workflow

The typical CMake workflow involves two main steps: **configuration** and **building**.

### 1.1. Configuration: Generating Build Files

The configuration step processes your `CMakeLists.txt` files and generates the necessary build system files in a specified build directory. It's highly recommended to perform an "out-of-source" build, meaning your build files are kept separate from your source code.

**Standard Configuration Command (Out-of-Source Build):**

The most modern and recommended way to configure a CMake project is using the `-B` and `-S` flags.

```bash
cmake -S /path/to/source -B /path/to/build
```

-   `-S /path/to/source`: Specifies the path to your source directory, which contains the top-level `CMakeLists.txt` file.
-   `-B /path/to/build`: Specifies the path to the directory where CMake will generate the build files. This directory will be created if it doesn't exist.

**Example:**

If your source code is in `~/my_project/src` and you want to create a `build` directory inside `~/my_project`:

```bash
cd ~/my_project
mkdir build
cmake -S src -B build
```

Alternatively, a common older approach, often seen in tutorials, is to navigate into the build directory first:

```bash
cd /path/to/build
cmake /path/to/source
```

**Example (older style):**

```bash
cd ~/my_project
mkdir build
cd build
cmake ../src
```
Both methods achieve the same result of generating build files in the specified build directory. The `-S -B` method is generally preferred for clarity and automation.

### 1.2. Building: Compiling Your Project

After configuration, you use the native build tool (e.g., `make` for Makefiles, `ninja` for Ninja, or `msbuild` for Visual Studio) to compile your project. CMake provides a convenient wrapper command, `cmake --build`, that abstracts away the specific build tool.

```bash
cmake --build /path/to/build_directory
```

-   `--build /path/to/build_directory`: Tells CMake to invoke the native build tool within the specified build directory.

**Example:**

Continuing from the previous example, to build your project:

```bash
cd ~/my_project
cmake --build build
```

This command will automatically detect whether you're using Makefiles, Ninja, etc., and run the appropriate build command (e.g., `make` or `ninja`).

## 2. Controlling Build Type (Debug, Release)

CMake allows you to specify the build type, which influences compilation flags for optimization and debugging.

**Setting the Build Type during Configuration:**

You use the `-D CMAKE_BUILD_TYPE=<Type>` option during the `cmake` configuration step.

```bash
cmake -S /path/to/source -B /path/to/build -D CMAKE_BUILD_TYPE=Debug
```

Common build types include:
-   `Debug`: Enables debugging symbols (`-g`) and disables optimizations (`-O0`). Ideal for development and debugging.
-   `Release`: Enables optimizations (`-O3`, `-Os`, etc.) and typically disables debugging symbols. Ideal for deployment.
-   `RelWithDebInfo`: Enables optimizations but also includes debugging symbols. Useful for profiling and debugging released versions.
-   `MinSizeRel`: Optimizes for smallest code size (`-Os`) and disables debugging symbols.

**Example: Creating a Debug Build:**

```bash
cd ~/my_project
mkdir build_debug
cmake -S src -B build_debug -D CMAKE_BUILD_TYPE=Debug
cmake --build build_debug
```

## 3. Listing Targets

CMake projects can define multiple "targets" (executables, libraries, etc.). You can list available targets for a given build directory.

**Listing all targets:**

```bash
cmake --build /path/to/build_directory --target help
```

This command will print a list of all available targets defined in your `CMakeLists.txt` files, along with brief descriptions.

**Example:**

```bash
cd ~/my_project
cmake --build build --target help
```

## 4. Building Specific Targets

Once you know the target name, you can build just that target instead of the entire project.

```bash
cmake --build /path/to/build_directory --target <target_name>
```

**Example:**

If you have an executable target named `my_app`:

```bash
cd ~/my_project
cmake --build build --target my_app
```

## 5. Enabling AddressSanitizer (ASan) for Debugging

AddressSanitizer (ASan) is a powerful memory error detector. Enabling it with CMake is straightforward by passing the appropriate compiler flags.

**Enabling ASan during Configuration:**

You typically enable ASan by adding `"-fsanitize=address"` to `CMAKE_CXX_FLAGS` (for C++) and `CMAKE_C_FLAGS` (for C). It's best combined with a `Debug` build type to avoid conflicts with optimizations.

```bash
cmake -S /path/to/source -B /path/to/build_asan \
      -D CMAKE_BUILD_TYPE=Debug \
      -D CMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer" \
      -D CMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer"
```
-   `-fsanitize=address`: Enables AddressSanitizer.
-   `-fno-omit-frame-pointer`: Crucial for ASan to produce accurate stack traces during error reports.

**Example: Configuring and Building with ASan:**

```bash
cd ~/my_project
mkdir build_asan
cmake -S src -B build_asan \
      -D CMAKE_BUILD_TYPE=Debug \
      -D CMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer" \
      -D CMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer"

cmake --build build_asan
```

When you run an executable built with ASan, it will automatically report memory errors to stderr.

## 6. Cleaning the Build Directory

To remove all generated build files and artifacts, you can use the `clean` target.

```bash
cmake --build /path/to/build_directory --target clean
```

**Example:**

```bash
cd ~/my_project
cmake --build build --target clean
```

Alternatively, you can simply delete the entire build directory:

```bash
rm -rf /path/to/build_directory
```

This is often the most reliable way to ensure a completely fresh build.

## 7. Advanced Configuration Options

You can pass any CMake variable as a definition using `-D`. This is commonly used for:

-   **Enabling/Disabling Features**:
    ```bash
    cmake -S src -B build -D BUILD_TESTS=ON
    ```
    (where `BUILD_TESTS` is a variable defined in your `CMakeLists.txt`)

-   **Specifying Install Prefix**:
    ```bash
    cmake -S src -B build -D CMAKE_INSTALL_PREFIX=/usr/local/my_app
    ```

-   **Choosing a Specific Generator**:
    By default, CMake tries to pick a suitable generator for your system (e.g., Makefiles on Linux, Visual Studio on Windows). You can explicitly specify one using the `-G` flag.

    ```bash
    cmake -S src -B build -G "Ninja"
    ```
    Common generators include:
    -   `Unix Makefiles`
    -   `Ninja`
    -   `Visual Studio 17 2022` (or other versions)
    -   `Xcode`

## Conclusion

Mastering these fundamental CMake commands will significantly streamline your C++ development workflow. By consistently using out-of-source builds, understanding build types, and leveraging tools like ASan, you can efficiently configure, build, and debug your projects across various platforms.
```