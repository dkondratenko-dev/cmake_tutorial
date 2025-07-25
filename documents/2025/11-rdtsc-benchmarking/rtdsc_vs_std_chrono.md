# Why Fintech Systems Use `RDTSC` Instead of `std::chrono` for Benchmarking

High-performance trading systems demand **nanosecond-level latency measurements**. In these environments, developers prefer using **low-level hardware instructions** such as `RDTSC` over high-level abstractions like `std::chrono`.

---

## `std::chrono`: High-Level and General-Purpose

**Pros:**

* Easy to use and read
* Cross-platform and portable
* Suitable for general-purpose performance analysis

**Cons:**

* Relatively high overhead
* Limited precision (typically microseconds)
* May involve system calls or be affected by OS scheduler
* Not suitable for per-function or per-instruction level measurements

## Chrono types

Of course. Let's break down the clock types available in the C++ `<chrono>` library. Understanding the difference is crucial for writing correct and robust time-dependent code.

In essence, a **clock** is a class that provides access to the current point in time (a `time_point`). The key is that different clocks measure time in different ways and for different purposes.

The C++ standard library defines three primary clocks.

---

### 1. `std::system_clock`

This is the "wall-clock" time. It represents the real-world time that you would see on a clock on the wall.

* **Purpose:** To relate a point in time to the real-world calendar and time of day.
* **Characteristics:**
  * **Represents System Time:** It is synchronized with the system's clock.
  * **NOT Monotonic:** This is its most important and dangerous characteristic. The time from `system_clock` can go backward! This can happen if the user manually changes the system time, or if the system synchronizes with an NTP (Network Time Protocol) server.
  * **Epoch:** Its epoch (the time point from which it starts counting) is typically the UNIX epoch: 00:00:00 UTC on January 1, 1970.
  * **Convertible:** It's the only clock that has `to_time_t()` and `from_time_t()` methods, making it easy to convert to and from legacy C-style time structures used for calendar functions.

* **When to Use It:**
  * Getting the current date and time to display to a user.
  * Creating timestamps for logging or file creation - plot performance trends over time, compare benchmarks done on different systems or OSes, or sync results with CI/CD pipelines.
  * Scheduling an event to happen at a specific calendar time (e.g., run a backup at 2:00 AM).
  * Also use when ultra-precise CPU cycle-level profiling is not needed. All that is needed is how long code runs in real-world time - Works across threads, processes, or even machines—helpful in distributed benchmarking.
  * Baseline alignment - When running multiple algorithms, especially in a long test suite, you can timestamp each segment to align them in time. This helps answer questions like, “Did performance degrade after an update?” or “Was the slowdown tied to system load at a particular moment?”

* **When NOT to Use It:**
  * **Never use it for measuring time intervals (durations).** If the clock is adjusted backward while you're timing something, you could get a negative duration, leading to bugs or infinite loops. When clock adjustments are possible, use std::chrono::steady_clock (which is monotonic and immune to clock changes)
  * **Not for performance-critical measurements - resolution is at seconds/milliseconds.** For those, use std::chrono::high_resolution_clock

* **Code Example:**

    ```cpp
    #include <iostream>
    #include <chrono>
    #include <ctime>

    int main() {
        // Get the current time point from the system_clock
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

        // Convert it to a legacy time_t to print it
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << "Current time: " << std::ctime(&now_c);
    }
    ```

    ```cpp
    #include <iostream>
    #include <chrono>
    #include <ctime>

    int main() {
        auto start = std::chrono::system_clock::now();

        // Code to benchmark
        for (volatile int i = 0; i < 1e7; ++i);

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        std::time_t start_time = std::chrono::system_clock::to_time_t(start);
        std::cout << "Started at: " << std::ctime(&start_time)
                  << "Elapsed time: " << elapsed_seconds.count() << "s\n";
    }
    ```

To convert Unix Time Stamps use https://www.unixtimestamp.com/ Epoch and unix timestamp converter for developers. Date and time function syntax reference for various programming languages.
What is the unix time stamp?
The unix time stamp is a way to track time as a running total of seconds. This count starts at the Unix Epoch on January 1st, 1970 at UTC. Therefore, the unix time stamp is merely the number of seconds between a particular date and the Unix Epoch.
---

### 2. `std::steady_clock`

This is the clock you should use for measuring time intervals.

* **Purpose:** To measure durations accurately.
* **Characteristics:**
  * **Monotonic:** This is its defining feature. Time from a `steady_clock` will **never go backward**. A later call to `now()` will always return a value greater than or equal to a previous call.
  * **Not Related to Wall Time:** The value of a `steady_clock::time_point` is not meaningful on its own. It's typically the time since the system booted or some other arbitrary starting point. You can't convert it to a calendar date. Its only purpose is to be subtracted from another `steady_clock::time_point`.

* **When to Use It:**
  * Timing how long a function takes to execute (benchmarking).
  * Implementing timeouts.
  * Controlling animations or physics loops in a game engine.
  * Anywhere you need to calculate an elapsed time (`end - start`).

* **Code Example:**

    ```cpp
    #include <iostream>
    #include <chrono>
    #include <thread> // For std::this_thread::sleep_for

    void long_running_task() {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    int main() {
        // Get the start time
        auto start = std::chrono::steady_clock::now();

        long_running_task();

        // Get the end time
        auto end = std::chrono::steady_clock::now();

        // Calculate the duration
        std::chrono::duration<double, std::milli> elapsed_ms = end - start;

        std::cout << "Task took " << elapsed_ms.count() << " ms.\n";
    }
    ```

---

### 3. `std::high_resolution_clock`

This clock is intended to be the one with the smallest possible tick period (the highest precision).

* **Purpose:** To provide the most precise time measurement available on the system.
* **Characteristics:**
  * **It's often just an alias!** On most modern C++ standard library implementations, `std::high_resolution_clock` is simply a `typedef` (alias) for `std::steady_clock`. The standard does not require it to be monotonic, but it usually is.
  * **Avoid if possible:** Because its behavior isn't strictly guaranteed and it's usually just `steady_clock` anyway, it is considered best practice to **prefer `std::steady_clock`** directly when you need to measure intervals. This makes your intent clearer.

* **When to Use It:**
  * In practice, it's better to use `std::steady_clock` for measuring time and `std::system_clock` for calendar time. There is rarely a good reason to use `high_resolution_clock` directly in modern code.

---

### Summary Table

| Clock | Monotonic? | Represents | Primary Use Case |
| :--- | :---: | :--- | :--- |
| `std::system_clock` | **No** | Wall-clock / Calendar time | Timestamps, scheduling |
| `std::steady_clock` | **Yes** | An arbitrary, steady time flow | Measuring intervals, timeouts, benchmarking |
| `std::high_resolution_clock` | Usually | (Often an alias for `steady_clock`) | (Legacy) High-precision measurements |

### The Golden Rule

* If you need to know **"how long"** something took, use **`std::steady_clock`**.
* If you need to know **"what time"** something happened, use **`std::system_clock`**.

### C++20 and Beyond

C++20 introduced new, more specialized clocks for handling time zones and different time standards with greater accuracy, including:

* `std::chrono::utc_clock`: Represents Coordinated Universal Time (UTC). It correctly handles leap seconds, unlike `system_clock`.
* `std::chrono::tai_clock`: Represents International Atomic Time (TAI), which is not subject to leap seconds.
* `std::chrono::gps_clock`: Represents the time standard used by the Global Positioning System (GPS).

For most everyday application development, `system_clock` and `steady_clock` remain the essential workhorses. The new clocks are for applications that require a very high degree of calendrical or scientific accuracy.

---

## `RDTSC`: Low-Level, Cycle-Accurate Timing

**Pros:**

* Reads the CPU's Time Stamp Counter (TSC)
* Extremely low overhead (no syscall)
* Accuracy up to CPU clock cycle
* Suitable for measuring sub-microsecond latency

**Cons:**

* Depends on CPU architecture
* Requires serialization (see below)
* Can be inaccurate across cores or frequency changes

**Typical Use Cases in Fintech:**

* Benchmarking order book handling
* Measuring latency of FIX message processing
* Profiling time-critical path down to CPU cycles

---

## Code Example: Timing with `RDTSC` and `cpuid`

```cpp
#include <iostream>
#include <x86intrin.h>

inline uint64_t rdtsc_start() {
    unsigned dummy;
    __asm__ __volatile__("cpuid" : "=a"(dummy) : "a"(0) : "ebx", "ecx", "edx");
    return __rdtsc();
}

inline uint64_t rdtsc_end() {
    unsigned dummy;
    uint64_t tsc = __rdtscp(&dummy);
    __asm__ __volatile__("cpuid" : "=a"(dummy) : "a"(0) : "ebx", "ecx", "edx");
    return tsc;
}
```

---

# Detailed Explanation of

```cpp
__asm__ __volatile__("cpuid" : "=a"(dummy) : "a"(0) : "ebx", "ecx", "edx");
```

This is a GCC inline assembly instruction used to **serialize** instruction execution before or after reading the CPU time-stamp counter.

### Purpose of `cpuid`

* Ensures that all previous instructions complete before continuing
* Prevents the CPU from reordering instructions around `RDTSC`
* Acts as a full serialization barrier

### Explanation of Syntax

| Part                    | Meaning                                                                    |
| ----------------------- | -------------------------------------------------------------------------- |
| `__asm__`               | GCC keyword to embed inline assembly                                       |
| `__volatile__`          | Prevents compiler from optimizing or reordering this instruction           |
| `"cpuid"`               | The assembly instruction to execute (CPU info + serialization)             |
| `: "=a"(dummy)`         | Output operand: result from `EAX` is written to `dummy`                    |
| `: "a"(0)`              | Input operand: place constant 0 into `EAX` before executing `cpuid`        |
| `: "ebx", "ecx", "edx"` | Clobber list: these registers are modified, so compiler can't rely on them |

### Operand Modifiers

* `"=a"`: output into the `EAX` register
* `"a"`: input to the `EAX` register
* `"ebx"`, `"ecx"`, `"edx"`: declare that these registers will be used and possibly changed

---

## Summary of Registers Used

| Register | Purpose                                                        |
| -------- | -------------------------------------------------------------- |
| `EAX`    | Input/output for CPU feature ID (set to 0 here for basic info) |
| `EBX`    | Used and modified by `cpuid`, needs to be clobbered            |
| `ECX`    | Same                                                           |
| `EDX`    | Same                                                           |

## How to convert CPU cycles to time

## 1\. What is a Clock Cycle in Simple Terms?

Imagine the processor as an orchestra conductor. Each clock cycle is a wave of the conductor's baton, after which all instruments (processor operations) act simultaneously. The clock frequency (the number of "waves" per second) determines how fast the processor works.

## 2\. How the Duration of One Clock Cycle is Calculated

**Formula:**

```
1 clock cycle = 1 / processor frequency
```

**For example:**

| Frequency | Duration of one clock cycle |
|-----------|-----------------------------|
| 1 GHz     | 1 / 1,000,000,000 = 1 ns    |
| 2.5 GHz   | 1 / 2,500,000,000 = 0.4 ns  |
| 4.2 GHz   | 1 / 4,200,000,000 ≈ 0.238 ns|

---

## Where to Learn More

### Intel Manuals

* **Intel® 64 and IA-32 Architectures Software Developer’s Manual**
  Volume 2 (Instruction Set Reference):
  [https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
  Look up: `CPUID`, `RDTSC`, `RDTSCP`

### GCC Inline Assembly

* GCC Extended Asm Syntax:
  [https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html)

### Optimization and Performance Guides

* Agner Fog’s Optimization Manuals:
  [https://www.agner.org/optimize/](https://www.agner.org/optimize/)

* LLVM/Clang: Precise Performance Analysis PDF:
  [https://llvm.org/devmtg/2020-09/slides/UsingLLVMForPerformanceAnalysis.pdf](https://llvm.org/devmtg/2020-09/slides/UsingLLVMForPerformanceAnalysis.pdf)
