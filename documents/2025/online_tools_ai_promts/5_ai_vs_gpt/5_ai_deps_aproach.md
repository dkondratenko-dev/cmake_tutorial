# De-Windowfication Strategy for a Large C++ Codebase

## Goal

The primary objective is to establish an optimal order for processing files and folders to remove Windows-specific dependencies (e.g., Win32, `windows.h`) without disrupting the build process or encountering unresolved dependencies. This involves identifying a "tree structure" of files or folders that can be refactored first, effectively acting as a topological sort of the codebase's dependencies.

## Tools & Techniques

### 1. Static Dependency Analysis

Understanding the interdependencies between files and modules, and pinpointing Windows-specific includes, is a critical initial step.

* **Include-what-you-use (IWYU)**: This Google tool analyzes `#include` dependencies and can generate dependency graphs. It excels at identifying unnecessary Windows-specific includes and can help determine files with the fewest Windows dependencies. IWYU can also be used in the suggested workflow by running `include-what-you-use -Xiwyu --mapping_file=iwyu.imp yourfile.cpp` after generating a compilation database.
* **Clang-based Tools (LibTooling, AST Matchers, `clangd`)**: Custom tools can be written using `libclang` or the Clang AST to parse the codebase, offering fine-grained control over tracking relationships. These tools are useful for extracting dependency information specific to de-windowfication needs. `clangd` can be used with `compile_commands.json`. `Clang-Tidy` with custom checks can identify specific Windows API usage or problematic patterns.
* **CppDepend**: A commercial tool for dependency analysis that creates detailed dependency matrices and visualizes circular dependencies, aiding in identifying problematic areas and bottlenecks.
* **Understand by SciTools**: This powerful tool for C++ parsing, dependency analysis, and visualization can generate call graphs, include graphs, and identify component dependencies.
* **Doxygen (with Graphviz)**: Primarily for documentation, Doxygen can generate include graphs and call graphs, providing a basic understanding of header dependencies. When using Doxygen, ensure `INCLUDE_GRAPH` and `CALL_GRAPH` are enabled. Doxygen can be run via `doxygen Doxyfile`.
* **`cppcheck` and `Clang Static Analyzer`**: These tools inspect dependencies and can be used for general static code analysis.
* **`ripgrep`, `ack`, or `grep`**: These command-line tools are effective for finding Win32-related code, such as `#include <windows.h>` or API calls like `CreateFile`, `GetModuleHandle`, or `WinMain`.

### 2. Building a Dependency Graph

The process involves mapping all `#include` relationships, focusing on Windows-specific headers (e.g., `windows.h`, `winsock2.h`), and identifying direct and transitive dependencies. A directed graph of file dependencies should be built.

* **CMake's Dependency Scanning**: Leverage built-in dependency analysis if using CMake, as it offers native integration with your build system. CMake can export dependency graphs for external processing. You can use `cmake --graphviz=graph.dot` to generate a Graphviz dot file. A compilation database can be generated using `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .`. CMake is useful for understanding dependencies between different parts of the codebase.
* **Graphviz + Custom Scripts**: Parse source files to extract includes, then visualize the dependency tree. Python scripts with libraries like `networkx` can be used for graph analysis and to create visual representations. Once dependency data is extracted (e.g., as DOT files), Graphviz is essential for visualization. You can visualize dependencies using `dot -Tpng graph.dot -o graph.png`.
* **Custom Python/Clang Parser**: Develop custom parsers to extract dependency information.
* **Bazel Query**: If using Bazel, `bazel query` can be used to analyze dependencies.
* **Visual Studio Project Dependencies**: Built-in tools in Visual Studio can visualize project dependencies.
* **Analyze `Makefile`s or CMake files**: These explicitly define build dependencies, and parsers can extract this information.
* **Gephi / Cytoscape**: For extremely large and complex dependency graphs, these tools offer advanced layout algorithms and interactive exploration.
* **Boost Graph Library**: This library can be used to perform topological sorting once dependencies are converted into a Directed Acyclic Graph (DAG).

### 3. Classifying Code by Portability

Categorize code based on its dependency on the operating system:

* **Portable (OK)**: Code with no OS-specific dependencies.
* **Light Dependency (WARN)**: Code that uses platform-abstractable elements like I/O or file system operations.
* **Platform-Specific (FAIL)**: Code that directly uses Win32 API, GDI, Registry, or other Windows-specific features.

### 4. Tagging and Isolating Platform Code

* Introduce interfaces such as `IFileSystem`, `IThread`, or `IWindow`.
* Wrap Windows-specific code behind these interfaces.
* Limit the use of `#ifdef _WIN32` to only OS-specific files.
* Create platform abstraction layers for identified hotspots and define clear interfaces between platform-specific and generic code. Implement cross-platform alternatives.

### 5. Prioritizing Refactoring (Topological Sorting and Layered Approach)

The goal is to find a valid processing order by using topological sorting algorithms to find files with no Windows dependencies first, then files that only depend on already-processed files.

* **Start with Leaf Nodes**: Begin with files that other components depend on but have minimal or no outgoing dependencies themselves. These are often utility libraries.
* **Work Upward / Move Up the Dependency Tree**: Systematically process through the dependency tree, gradually addressing dependencies.
* **Layered Removal Strategy**: Refactor high-level, independent modules first, then gradually descend to core dependency-heavy components.
* **Identify Abstraction Layers**: Look for natural boundaries in your architecture (e.g., network layer, file I/O, threading, GUI) to create platform-neutral interfaces.
* **Create a Processing Queue**: Group related files/modules for efficient batch processing.
* **"De-Windowfication Scorecard"**: Track metrics for each file/folder (e.g., number of Windows API calls, Windows-specific headers, dependencies, complexity) to prioritize efforts.
* **Wrapper/Abstraction Layers**: For components with many dependencies, create abstraction layers or wrappers around Windows-specific functionality to allow de-windowfication of dependent components without immediately touching the core Windows-dependent code.
* **Identify Cycles**: Address dependency cycles, as they break the "tree" structure and require strategies like introducing interfaces or refactoring.

## Best Practices

* **Avoid Deep Nesting of `#ifdef`**: Do not deeply nest `#ifdef` blocks inside core logic.
* **Move OS-Specific Logic**: Relocate OS-specific logic to per-platform implementation files.
* **Consider Cross-Platform APIs**: Utilize standard C++ libraries like `std::filesystem` (C++17), `std::thread`, and `std::optional`/`std::variant`, as well as third-party cross-platform libraries like `Boost.Asio`, Qt, wxWidgets, or Dear ImGui for networking, threading, GUI, and utilities.
* **Set up CI for Cross-Platform Builds**: Implement Continuous Integration (CI) to test with cross-compilation or Linux builds, ensuring functionality on multiple platforms.
* **Dedicated Team/Resources**: Treat de-windowfication as a distinct project requiring dedicated resources.
* **Clear Definitions**: Define what constitutes a "de-windowfied" component (e.g., no Windows headers, no direct WinAPI calls, passes cross-platform tests).
* **Regular Code Reviews**: Conduct frequent code reviews to ensure adherence to new patterns and best practices.
* **Documentation**: Maintain thorough documentation of dependencies, refactoring decisions, adopted patterns, and areas still needing work.
* **Incremental Approach**: Break down the de-windowfication into small, manageable chunks instead of attempting it all at once.
* **"Shim" Layers**: For stubborn Windows-specific external dependencies, consider creating a "shim" layer to provide necessary interfaces on other platforms, even if it's a no-op or throws an error.
* **Feature Flags/Build System Options**: Use your build system to enable/disable parts of the codebase, allowing gradual transitions to de-windowfied versions.
* **Version Control (Git)**: Crucial for managing changes, branching, and merging. Frequent commits and small, focused pull requests are recommended.

## Suggested Workflow

1.  **Generate Compilation Database**: `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .`
2.  **Run IWYU**: `include-what-you-use -Xiwyu --mapping_file=iwyu.imp yourfile.cpp`
3.  **Generate Doxygen Graph**: `doxygen Doxyfile`
4.  **Detect Windows-specific Code**:
    * `rg "#include <windows.h>"`
    * `rg "CreateFile|GetModuleHandle|WinMain"`
5.  **Visualize Dependencies**: `dot -Tpng graph.dot -o graph.png`

## Example De-Windowfication Tree

Consider a hypothetical codebase structure:

```
Utils/
├── StringUtils.cpp         OK No Windows dependency
└── Logger.cpp              OK No Windows dependency

Core/
├── FileSystem.cpp          FAIL Uses Win32 API
├── FileSystem_Linux.cpp    OK POSIX-compatible
└── Config.cpp              OK Uses only Utils

UI/
└── Window.cpp              FAIL Win32-heavy (GDI, HWND, etc.)
```

**Recommended Order:**

1.  Refactor and move `Utils/` to a portable layer.
2.  Abstract `FileSystem` into an interface and implement a cross-platform version.
3.  Postpone `UI/` refactoring until the core is portable.

## Optional Enhancements

* Add `clang-tidy` checks for Windows-specific APIs.
* Use unit tests to verify platform-independent modules.
* Automate extraction of platform code statistics (Lines of Code, file count).
* Implement `Google Test` or `Catch2` for unit and integration tests.
* Integrate with CI/CD pipelines (e.g., GitHub Actions, Jenkins) for automated build tests.
* Use `CMake CTest` for running test suites.