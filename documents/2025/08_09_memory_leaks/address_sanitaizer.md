# Understanding, Identifying, and Fixing Memory Leaks in C++ Using AddressSanitizer (ASan)

Welcome! This document will guide you through understanding, identifying, and fixing memory leaks in C++ using AddressSanitizer (ASan). Transitioning from C# with its automatic garbage collection to C++ with explicit memory management can be challenging. In fintech applications, where high performance and low latency are crucial, efficient and correct memory usage is paramount.

AddressSanitizer is a powerful runtime memory error detector that can identify issues such as:

- Use after free
- Use after return (partial)
- Use after scope (partial)
- Double free
- Invalid free
- Memory leaks
- Out-of-bounds access (heap, stack, global)

---

## I. Prerequisites

To follow along, you'll need a Linux environment (e.g., Ubuntu) with:

- **g++** or **clang++** (GCC 4.8+ or Clang 3.1+ is required for ASan support)
- **CMake** (version 3.0+)
- **make** or **ninja** (build system)
- **VS Code** with the **CMake Tools** extension (recommended for an integrated experience)

You can install the compilers and CMake on Ubuntu/Debian with:

```bash
sudo apt update
sudo apt install build-essential cmake
```

For VS Code, install the "**C/C++ Extension Pack**" which includes "**CMake Tools**".

---

## II. The C++ Code (main.cpp)

This file contains two examples: a simple memory leak and a more complex one simulating trade data management.

```cpp
// main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory> // For std::unique_ptr, std::shared_ptr
#include <cstring> // For strcpy

// --- Fintech-related structures (for the advanced example) ---

struct Trade {
    std::string tradeId;
    std::string symbol;
    double price;
    long quantity;
    char* settlementInstructions;

    Trade(const std::string& id, const std::string& sym, double p, long q)
        : tradeId(id), symbol(sym), price(p), quantity(q), settlementInstructions(nullptr) {
        std::cout << "Trade " << tradeId << " created." << std::endl;
    }

    ~Trade() {
        if (settlementInstructions) {
            delete[] settlementInstructions;
            settlementInstructions = nullptr;
            std::cout << "Settlement instructions for " << tradeId << " freed." << std::endl;
        }
        std::cout << "Trade " << tradeId << " destroyed." << std::endl;
    }

    void setSettlementInstructions(const std::string& instructions) {
        if (settlementInstructions) {
            delete[] settlementInstructions;
        }
        settlementInstructions = new char[instructions.length() + 1];
        strcpy(settlementInstructions, instructions.c_str());
    }

    void display() const {
        std::cout << "  ID: " << tradeId << ", Symbol: " << symbol
                  << ", Price: " << price << ", Quantity: " << quantity;
        if (settlementInstructions) {
            std::cout << ", Instructions: " << settlementInstructions;
        }
        std::cout << std::endl;
    }
};

class TradeProcessor {
public:
    TradeProcessor() {
        std::cout << "TradeProcessor created." << std::endl;
    }

    ~TradeProcessor() {
        std::cout << "TradeProcessor destroyed." << std::endl;
    }

    Trade* processAndAllocateTrade(const std::string& id, const std::string& symbol, double price, long quantity) {
        Trade* newTrade = new Trade(id, symbol, price, quantity);
        newTrade->setSettlementInstructions("Standard T+2 settlement.");
        std::cout << "  [PROCESSOR] Allocated new Trade " << id << " on heap." << std::endl;
        return newTrade;
    }

    std::unique_ptr<Trade> createSmartTrade(const std::string& id, const std::string& symbol, double price, long quantity) {
        std::unique_ptr<Trade> smartTrade = std::make_unique<Trade>(id, symbol, price, quantity);
        smartTrade->setSettlementInstructions("Smart pointer managed trade.");
        std::cout << "  [PROCESSOR] Allocated smart Trade " << id << " on heap (unique_ptr)." << std::endl;
        return smartTrade;
    }
};

// --- Memory Leak Examples ---

void simpleMemoryLeak() {
    std::cout << "\n--- Running Simple Memory Leak Example ---" << std::endl;
    int* data = new int[10];
    std::cout << "  Allocated 10 ints. Forgetting to free it." << std::endl;
    data[0] = 100;
}

void advancedMemoryLeak_problematic() {
    std::cout << "\n--- Running Advanced Memory Leak (Problematic) Example ---" << std::endl;
    TradeProcessor processor;

    for (int i = 0; i < 3; ++i) {
        std::string tradeId = "TRADE_" + std::to_string(100 + i);
        Trade* t = processor.processAndAllocateTrade(tradeId, "AAPL", 175.50 + i, 100 + i*10);
        t->display();
    }

    Trade* anotherLeakyTrade = new Trade("LEAK_001", "MSFT", 400.0, 50);
    anotherLeakyTrade->setSettlementInstructions("Fast settlement required.");
    std::cout << "  [MAIN] Created another trade for potential re-assignment." << std::endl;

    anotherLeakyTrade = new Trade("LEAK_002", "GOOG", 150.0, 75);
    anotherLeakyTrade->setSettlementInstructions("Regular settlement.");
    std::cout << "  [MAIN] Overwritten pointer, LEAK_001 is now leaked." << std::endl;
}

void advancedMemoryLeak_fixed() {
    std::cout << "\n--- Running Advanced Memory Leak (FIXED) Example ---" << std::endl;
    TradeProcessor processor;

    std::vector<std::unique_ptr<Trade>> trades;
    for (int i = 0; i < 3; ++i) {
        std::string tradeId = "FIXED_TRADE_" + std::to_string(100 + i);
        trades.push_back(processor.createSmartTrade(tradeId, "IBM", 200.0 + i, 50 + i*5));
        trades.back()->display();
    }

    std::unique_ptr<Trade> goodTrade = std::make_unique<Trade>("FIXED_001", "AMZN", 180.0, 120);
    goodTrade->setSettlementInstructions("Smart pointer handles re-assignment.");
    std::cout << "  [MAIN] Created smart trade for re-assignment." << std::endl;

    goodTrade = std::make_unique<Trade>("FIXED_002", "NVDA", 1000.0, 10);
    goodTrade->setSettlementInstructions("New smart pointer assigned.");
    std::cout << "  [MAIN] Re-assigned smart pointer, old object properly deleted." << std::endl;
}

int main() {
    simpleMemoryLeak();
    advancedMemoryLeak_problematic();

    std::cout << "\n--- All problematic examples finished. Memory leaks occurred. ---" << std::endl;
    std::cout << "--- Now running fixed examples. ---" << std::endl;

    advancedMemoryLeak_fixed();

    std::cout << "\n--- All fixed examples finished. No memory leaks expected. ---" << std::endl;

    return 0;
}
```

### Explanation of main.cpp

* **Trade struct**: Represents a financial trade. It includes a `char* settlementInstructions` to demonstrate dynamic allocation within an object and the need for a proper destructor to free that memory.
* **TradeProcessor class**:
    * `processAndAllocateTrade()`: This function dynamically allocates a `Trade` object using `new` and returns a raw pointer. If the caller doesn't explicitly delete this pointer, a memory leak occurs.
    * `createSmartTrade()`: This function returns a `std::unique_ptr<Trade>`, ensuring that the allocated `Trade` object is automatically deallocated when the `unique_ptr` goes out of scope.
* **Memory Leak Examples**:
    * `simpleMemoryLeak()`: Allocates an array of integers using `new int[10]` but never calls `delete[]`.
    * `advancedMemoryLeak_problematic()`: Calls `processAndAllocateTrade()` in a loop, creating several `Trade` objects. Since the returned raw pointers are not stored or deleted, these become leaks.
    * `advancedMemoryLeak_fixed()`: Uses `std::unique_ptr` via `createSmartTrade()` and stores them in a `std::vector<std::unique_ptr<Trade>>`. When the vector or its elements go out of scope, memory is automatically freed.

---

## III. The CMake Configuration (CMakeLists.txt)

CMake simplifies the build process. This file tells CMake how to compile your project and enable AddressSanitizer.

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(FintechMemoryLeaks CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "Enabling AddressSanitizer for build type: ${CMAKE_BUILD_TYPE}")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()

add_executable(fintech_asan_example main.cpp)
```

### Explanation of CMakeLists.txt

* `cmake_minimum_required` and `project`: Standard CMake boilerplate.
* `set(CMAKE_CXX_STANDARD 17)`: Ensures C++17 features are available.
* `if(CMAKE_BUILD_TYPE STREQUAL "Debug" ...)`: Enables ASan only for `Debug` or `RelWithDebInfo` builds.
* `-fsanitize=address`: The main flag that tells `g++` (or `clang++`) to enable AddressSanitizer.
* `-fno-omit-frame-pointer`: Improves stack traces generated by ASan.
* `-g`: Includes debug information, essential for ASan to provide readable stack traces.
* `add_executable(fintech_asan_example main.cpp)`: Defines your executable target.

---

## IV. Building the Project (Ubuntu + VS Code + CMake + g++)

Follow these steps to build your project with AddressSanitizer enabled:

1.  **Create Project Directory**: Create a new folder (e.g., `fintech_asan_project`) and place both `main.cpp` and `CMakeLists.txt` inside it.
2.  **Open in VS Code**: Open the `fintech_asan_project` folder in VS Code.
3.  **Configure CMake**:
    * VS Code's "**CMake Tools**" extension should automatically detect the `CMakeLists.txt` file.
    * In the VS Code bottom status bar, click on "**No Kit Selected**" and choose a C++ compiler.
    * Click the "**Build**" button in the status bar, or press `Ctrl+Shift+P` to open the Command Palette, type `CMake: Configure`, and select it.
4.  **Select Build Type**: In the VS Code status bar, click on the current build type and select **Debug** or **RelWithDebInfo**.
5.  **Build the Project**: Click the "**Build**" button in the VS Code status bar, or use the Command Palette: `CMake: Build`.

Alternatively, open a terminal in VS Code and execute:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

---

## V. Running with AddressSanitizer and Analyzing Output

Once built, run the executable. ASan automatically intercepts memory operations and reports errors to `stderr`.

1.  **Open Integrated Terminal**: In VS Code, open a new terminal.
2.  **Navigate to Build Directory**:
    ```bash
    cd build
    ```
3.  **Run the Executable**:
    ```bash
    ./fintech_asan_example
    ```
4.  **Observe ASan Output**: Look for lines starting with `==...==ERROR: AddressSanitizer: leak`.

### Expected ASan Output for simpleMemoryLeak()

```
--- Running Simple Memory Leak Example ---
Allocated 10 ints. Forgetting to free it.

==...==ERROR: AddressSanitizer: leak of 40 byte(s) in 0x...
#0 0x... in simpleMemoryLeak() .../main.cpp:92
#1 0x... in main .../main.cpp:168
#2 0x... in __libc_start_main (<null>:-1)
#3 0x... in _start (<null>:-1)

SUMMARY: AddressSanitizer: 40 byte(s) leaked in 1 allocation(s).
```

### Analysis for Simple Leak

* `leak of 40 byte(s)`: Indicates the total size of the leaked memory.
* `in simpleMemoryLeak() .../main.cpp:92`: Points to the line where the memory was allocated but never freed.

### Expected ASan Output for advancedMemoryLeak_problematic()

```
--- Running Advanced Memory Leak (Problematic) Example ---
[PROCESSOR] Allocated new Trade TRADE_100 on heap.
ID: TRADE_100, Symbol: AAPL, Price: 175.5, Quantity: 100, Instructions: Standard T+2 settlement.
[PROCESSOR] Allocated new Trade TRADE_101 on heap.
ID: TRADE_101, Symbol: AAPL, Price: 176.5, Quantity: 110, Instructions: Standard T+2 settlement.
[PROCESSOR] Allocated new Trade TRADE_102 on heap.
ID: TRADE_102, Symbol: AAPL, Price: 177.5, Quantity: 120, Instructions: Standard T+2 settlement.
[MAIN] Created another trade for potential re-assignment.
[MAIN] Overwritten pointer, LEAK_001 is now leaked.

==...==ERROR: AddressSanitizer: leak of 72 byte(s) in 0x...
#0 0x... in operator new(unsigned long) (<null>:-1)
#1 0x... in TradeProcessor::processAndAllocateTrade(std::string const&, std::string const&, double, long) .../main.cpp:81
#2 0x... in advancedMemoryLeak_problematic() .../main.cpp:106
#3 0x... in main .../main.cpp:169
#4 0x... in __libc_start_main (<null>:-1)

SUMMARY: AddressSanitizer: 288 byte(s) leaked in 4 allocation(s).
```

### Analysis for Advanced Leak

* Multiple `leak of 72 byte(s)` reports correspond to the `Trade` objects.
* The stack trace points to memory allocated by `new Trade(...)` on line 81 inside `processAndAllocateTrade`, which was leaked because the caller of `processAndAllocateTrade` failed to deallocate the returned pointer.

### Important ASAN_OPTIONS Environment Variable

By default, ASan might only report memory leaks on program exit. You can control its behavior using the `ASAN_OPTIONS` environment variable. For example, to always detect leaks:

```bash
export ASAN_OPTIONS=detect_leaks=1
./fintech_asan_example
```

Or, for detailed information:

```bash
export ASAN_OPTIONS=detect_leaks=1:log_path=./asan.log:fast_unwind_on_malloc=0
./fintech_asan_example
```

---

## VI. Fixing the Leaks and Understanding Smart Pointers

The key to fixing these leaks is using **smart pointers** from the C++ Standard Library, specifically `std::unique_ptr`.

### Fix for Simple Memory Leak

In `simpleMemoryLeak()`, add a `delete[]` call:

```cpp
void simpleMemoryLeak() {
    std::cout << "\n--- Running Simple Memory Leak Example ---" << std::endl;
    int* data = new int[10];
    std::cout << "  Allocated 10 ints. Will free it now." << std::endl;
    data[0] = 100;

    delete[] data;
    data = nullptr;
    std::cout << "  Freed 10 ints." << std::endl;
}
```

### Fix for Advanced Memory Leak

The `advancedMemoryLeak_fixed()` function demonstrates the proper use of `std::unique_ptr`.

* **Automatic Cleanup with `std::unique_ptr`**: Instead of returning raw pointers from `TradeProcessor::processAndAllocateTrade`, the `TradeProcessor::createSmartTrade` function returns a `std::unique_ptr<Trade>`. When the `unique_ptr` goes out of scope, it automatically calls `delete` on the managed object.
* **Storing `unique_ptr`s in a Container**: Using `std::vector<std::unique_ptr<Trade>> trades` ensures that when the vector is cleared or goes out of scope, each `unique_ptr` within it will be correctly destroyed.
* **Handling Re-assignment Safely**: When you re-assign a `std::unique_ptr`, the object previously managed by it is automatically deleted before the new object is assigned.

---

## VII. VS Code Tips for C++ Development

* **CMake Tools Extension**: Helps you configure, build, and debug your CMake projects directly from the VS Code UI.
* **C/C++ Extension**: Provides IntelliSense, code navigation, debugging, and other language features.
* **Debugging with ASan**: Configure VS Code debuggers (like GDB or LLDB) to work with ASan.
* **Tasks**: Define custom build/run tasks in `.vscode/tasks.json` to easily execute `cmake --build` or run your executable with `ASAN_OPTIONS` set.
* **settings.json**: Configure CMake-related settings in your workspace `.vscode/settings.json`.
