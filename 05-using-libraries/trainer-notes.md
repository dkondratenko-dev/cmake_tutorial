# 📘 CMake Tutorial — Structured Project with `add_library`, `add_subdirectory`, `target_link_libraries`

This tutorial explains the key CMake functions used in a structured C++ project, where:

- source code is in the `src/` folder,
- headers are in the `include/` folder,
- a separate `CMakeLists.txt` is used for the `src/` folder.

---

## 🏗️ Project Structure

05-using-libraries/
├── CMakeLists.txt
├── include/
│   └── hello.h
├── src/
│   ├── CMakeLists.txt
│   └── hello.cpp
└── main.cpp

---

## 🔧 CMake Functions and Their Explanations

### 📌 `add_library(hello hello.cpp)`

- **Purpose**: Creates a library target named `hello` using the file `hello.cpp`.
- **Type**: This creates a *static* library by default.
- **Usage**:

```cmake
  add_library(<name> <source1> <source2> ...)

```

- **Example**:

  ```cmake
  add_library(hello hello.cpp)
  ```

- 💡 To create a shared library:

  ```cmake
  add_library(hello SHARED hello.cpp)
  ```

---

### 📌 `target_include_directories(hello PUBLIC ${PROJECT_SOURCE_DIR}/include)`

- **Purpose**: Tells the compiler where to find header files for the target `hello`.
- **Syntax**:

  ```cmake
  target_include_directories(<target> <scope> <dir1> <dir2> ...)
  ```

- **Scopes**:

  - `PRIVATE`: only for this target
  - `PUBLIC`: for this target and for targets that link to it
  - `INTERFACE`: only for targets that link to this one
- **Example**:

  ```cmake
  target_include_directories(hello PUBLIC ${PROJECT_SOURCE_DIR}/include)
  ```

  This means:

  - When building `hello`, use headers from `include/`
  - When other targets link to `hello`, they will also get this include path

---

### 📌 `target_link_libraries(main PRIVATE hello)`

- **Purpose**: Links the `hello` library to the `main` executable.
- **Syntax**:

  ```cmake
  target_link_libraries(<target> <scope> <library> ...)
  ```

- **Scopes**:

  - `PRIVATE`: link only for this target
  - `PUBLIC`: link and propagate include paths
  - `INTERFACE`: link for dependents only
- **Example**:

  ```cmake
  target_link_libraries(main PRIVATE hello)
  ```

---

### 📌 `add_subdirectory(src)`

- **Purpose**: Tells CMake to enter the `src/` folder and process its `CMakeLists.txt`.
- **Usage**:

  ```cmake
  add_subdirectory(<dir>)
  ```

- **Effect**: Makes targets defined in `src/CMakeLists.txt` (like the `hello` library) available to the root CMake project.

---

### 📌 `include_directories(include)`

- **Purpose**: Adds the `include/` folder to the global include path.
- **Usage**:

  ```cmake
  include_directories(<dir1> <dir2> ...)
  ```

- **Important**: This is a *global* command and not target-specific.
  It is recommended to prefer `target_include_directories()` for modern CMake projects.

---

## 💡 Summary: Modern Best Practice

| Task                  | Recommended Command                        |
| --------------------- | ------------------------------------------ |
| Create a library      | `add_library()`                            |
| Link libraries        | `target_link_libraries()`                  |
| Set include path      | `target_include_directories()` (preferred) |
| Add subfolder project | `add_subdirectory()`                       |

---

## ✅ Final Result: Output from `./main`

```bash
Hello from src/!
```
