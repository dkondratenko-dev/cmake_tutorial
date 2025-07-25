# Advanced Performance Topics in C++

This lecture covers three important performance-oriented topics:

1. **Cache-friendly data structures**
2. **Inline assembly (basic)**
3. **Memory ordering**

---

## 1. Cache-Friendly Data Structures

### 1.1 Why Cache Matters

Modern CPUs are much faster than main memory. Accessing data from the L1 cache is ~1 ns, while accessing RAM can be 100x slower. Designing data structures that fit well into cache can greatly improve performance.

### 1.2 CPU Cache Access Time

Providing "exact" access times for CPU cache is difficult because the numbers depend heavily on the specific CPU architecture (Intel, AMD, Apple), its clock speed, and even the current workload. However, we can provide typical, averaged values that perfectly illustrate the order-of-magnitude differences.

Here are the approximate data access times for a modern CPU:

| Memory Level | Typical Access Time (Latency) | Explanation & Analogy |
| :--- | :--- | :--- |
| **CPU Registers** | < 1 nanosecond (1 clock cycle) | **Blazing Fast.** Like having the data directly in your hands. |
| **L1 Cache** | ~1-2 nanoseconds (1-3 clock cycles) | **Extremely Fast.** Like grabbing a tool from your desk. |
| **L2 Cache** | ~5-10 nanoseconds (4-10 clock cycles) | **Very Fast.** Like getting a tool from a cabinet in the same room. |
| **L3 Cache** | ~10-40 nanoseconds (10-40 clock cycles) | **Fast.** Like walking to a storage closet in the next room. |
| **Main Memory (RAM)** | ~60-100 nanoseconds | **Slow.** Like driving to a hardware store across town to get a tool. |
| **SSD (Solid-State Drive)**| ~0.1-1 millisecond (100,000-1,000,000 ns) | **Very Slow.** Like ordering a tool for delivery from another country. |
| **HDD (Hard Disk Drive)** | ~5-10 milliseconds (5,000,000-10,000,000 ns)| **Extremely Slow.** Like waiting for a custom tool to be manufactured and shipped. |

### 1.3 Key Takeaways

* **Order of Magnitude:** Notice that the difference between levels is not linear. L2 access is roughly 3-5 times slower than L1, but main memory access can be 10-20 times slower than L3. The difference between L1 cache and accessing data on a hard drive can be millions of times.
* **Cache Miss:** When the CPU fails to find the data it needs in the L1 cache, it looks in L2, then L3, and only then does it access slow main memory. Each such "miss" and transition to a lower level in the memory hierarchy results in significant delays (stalls) during which the CPU is idle.
* **Why This Matters for a Programmer:** The goal of writing high-performance code is to organize data and algorithms so that the CPU finds the necessary information in the L1 and L2 caches as often as possible. Techniques like **cache locality** (sequential data access in an array) and **cache-friendly data structures** (SoA vs. AoS) are aimed directly at achieving this.

Therefore, while the exact numbers can vary, the relative speed differences remain consistent and are a fundamental principle of modern computer architecture.

### 1.4 Principles of Cache-Friendly Design

* **Contiguous memory layout:** Store related data sequentially.
* **Avoid pointer chasing:** Linked lists and trees often scatter nodes in memory, leading to cache misses.
* **Struct of Arrays (SoA) vs. Array of Structs (AoS):**
  * AoS:

        ```cpp
        struct Particle {
            float x, y, z;
            float velocity;
        };
        std::vector<Particle> particles;
        ```

        This is cache-friendly when accessing all fields of each particle.
  * SoA:

        ```cpp
        struct ParticleData {
            std::vector<float> x, y, z, velocity;
        };
        ```

        This is better when processing only one field (e.g., all `x`).
* **Use `std::vector` or `std::array` instead of `std::list` or `std::map`** when possible.
* **Reserve memory upfront:**

    ```cpp
    std::vector<int> v;
    v.reserve(1000); // avoid multiple reallocations
    ```

### 1.5 Example: Improving a Linked List

    ```cpp
    // Slow (cache unfriendly)
    struct Node {
        int value;
        Node* next;
    };

    // Better: store in contiguous memory
    std::vector<int> values;
    ```

### 1.6 Exercises

* Convert a `std::list<int>` to `std::vector<int>` and measure performance.
* Implement both AoS and SoA for 1 million particles and benchmark.

---

## 2. Inline Assembly (Basic Level)

### 2.1 What is Inline Assembly?

Inline assembly allows embedding assembly instructions directly in C++ code. It is mostly used for:

* Accessing CPU instructions not exposed in intrinsics.
* Low-level optimizations or debugging.

### 2.2 Syntax (GCC/Clang)

```cpp
    int x = 5, y;
    asm("mov %1, %0" : "=r"(y) : "r"(x));
```

* `%0`, `%1` refer to output/input operands.
* `"=r"(y)` means output is in a general register.
* `"r"(x)` means input is a register containing `x`.

### 2.3 Syntax (MSVC)

```cpp
    __asm {
        mov eax, 5
        add eax, 2
    }
```

### 2.4 Example: Reading CPU Timestamp Counter

```cpp
    unsigned long long read_tsc() {
        unsigned int lo, hi;
        asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
        return ((unsigned long long)hi << 32) | lo;
    }
```

### 2.5 When to Avoid Inline Assembly

* Modern compilers (after 2000's) and intrinsics often produce better code. -O0 up to -O3
* Inline assembly is not portable.
* Use only when absolutely necessary.

### 2.6 Exercise

* Write an inline assembly function to add two integers.
* Compare performance to a regular C++ function (likely the same).

---

## 3. Memory Ordering in C++ (with `std::atomic`)

When multiple threads are reading and writing shared data, the compiler and CPU are free to reorder instructions for optimization, **unless we explicitly impose rules using memory ordering**. C++ provides several memory ordering models with `std::atomic` operations.

This document summarizes the key ideas and provides examples for better understanding.

### 3.1 Why Memory Ordering Exists

Modern CPUs and compilers can:

* Reorder instructions for better performance (out-of-order execution).
* Keep writes in buffers that may not be immediately visible to other threads.

This means that without proper synchronization, **one thread might see stale or incomplete data**.

**Memory orderings** define what guarantees are made about visibility and ordering of operations between threads.

### 3.2 Atomic Operations

An **atomic operation** is an operation that is performed as a single, indivisible step — it **cannot be interrupted or observed in a partial state** by other threads. In other words, when one thread performs an atomic operation, no other thread can see an intermediate or "half-done" result of that operation.

#### 3.2.1 Key Properties of Atomic Operations

1. **Indivisibility:**
    The operation runs to completion without being interrupted. For example, if one thread does `atomicInt.store(5)`, no other thread will ever see a value in-between (like 2 or 3) — they will see either the old value or the new value (5).

2. **Consistency Across Threads:**
    All threads see the result of atomic operations in a well-defined order (depending on the chosen memory ordering, e.g., `memory_order_seq_cst`).

3. **Thread-Safety:**
    Unlike normal `int` or `bool` variables, you can safely read or modify `std::atomic<int>` from multiple threads **without additional locks**.

#### 3.2.2 Examples of Atomic Operations

* **Atomic store:**

```cpp
    std::atomic<int> x{0};
    x.store(10);  // Atomically sets x to 10
```

* **Atomic load:**

```cpp
    int value = x.load();  // Atomically reads x
```

* **Atomic read-modify-write:**
    Operations like `fetch_add`, `fetch_sub`, `exchange`, or `compare_exchange` are atomic.

```cpp
    x.fetch_add(1);  // Atomically increments x by 1
```

#### 3.2.3 Why Not Just Use `int`?

On many platforms, `int x = x + 1;` is **not atomic** because it's actually multiple machine instructions:

1. Load `x` from memory.
2. Add 1.
3. Store the result back to memory.

If two threads run this simultaneously, they can overwrite each other's updates (called a **data race**). So two threads can simultaneously increment a location from 0 to 1 but the result of the two increments will be 1 instead of 2. With `std::atomic<int>`, the increment is a single atomic machine instruction.

#### 3.2.4 Next Step

Would you like me to create a **very simple visual example** (with 2 threads incrementing a variable, one with `int`, one with `std::atomic<int>`) so you can show your student why atomic operations matter?

Random example.

```cpp
    class Ob{};

    Ob createOb()
    {
        Ob o;
        o.setData();
        //Which return should we use?
        return o;
        return std::move(o);//Not good for returning a result because it can degrade performance
                            //If Ob has many fields of simpler datatypes the move can translate to much copying
    }

    int main()
    {
        Ob o = createOb();
    }
```

Most likely a compiler feature called Return Value Optimization (RVO) is used to optimize the code to:

```cpp
    class Ob{};

    Ob o;
    Ob createOb()
    {//RVO eliminates the need to return the result!
        Ob o;
        o.setData();
    }

    int main()
    {
        Ob o = createOb();//
    }
```

### 3.3 Types of Memory Orderings

C++ offers the following memory orderings (used in `atomic` operations):

#### 3.3.1 `memory_order_relaxed`

* **Guarantee:** No synchronization, no ordering. Only guarantees that the operation itself is atomic.
* **Use case:** When you don't need ordering (e.g., counters that are read only for statistics).
  
```cpp
    std::atomic<int> counter = 0;
    counter.fetch_add(1, std::memory_order_relaxed); // Just atomic increment

    //Should this be corrected to:

    //Declare at global level
    std::atomic<int> counter = 0;

    //Thread A
    counter.fetch_add(1, std::memory_order_relaxed); 

    //Thread B
    counter.fetch_add(1, std::memory_order_relaxed); 
```

#### 3.3.2 `memory_order_release` (for `store`)

* **Guarantee:** All writes/reads before this store in the same thread are completed **before** the atomic store is visible to other threads.
* **Use case:** Publish data and then set a flag.

```cpp
    std::atomic<bool> ready {false};
    int data{0};
    // Thread A
    data = 42; // Some shared data
    ready.store(true, std::memory_order_release); // Make data visible

    // With memory_order_relaxed compiler can rearrange above code so the line for data=42 is executed after the call to ready.store.
    // Thread B: If another thread has the code below
    while (!ready.load(std::memory_order_acquire)) {} //Spin-wait
    int x = data; // Guaranteed to see the correct value of data - we expected to see 42 here but because the compiler rearranged the line for setting data = 42, we will set x to 0
```

#### 3.3.3 `memory_order_acquire` (for `load`)

* **Guarantee:** All reads/writes after this load in the same thread happen **after** the atomic load.
* **Use case:** Wait for the release flag before reading the data.

```cpp
    // Thread B
    while (!ready.load(std::memory_order_acquire)) {} // Spin-wait
    int x = data; // Guaranteed to see the correct value of data
```

**Release + Acquire** together establish a **happens-before** relationship.

#### 3.3.4 `memory_order_acq_rel` (for read-modify-write operations)

* **Guarantee:** Combines `acquire` for loads and `release` for stores.
* **Use case:** Atomic operations like `fetch_add` that both read and write.

#### 3.3.5 `memory_order_seq_cst` (Sequentially Consistent)

* **Guarantee:** The strongest ordering; all atomic operations appear in a single global order.
* **Use case:** When you want simple reasoning at the cost of performance.
* **Default:** This is the default when ordering is not specified in code.
  
```cpp
    ready.store(true, std::memory_order_seq_cst);
```

### 3.4 Key Principle: What Is Synchronized?

**Important:** `memory_order_release` does not force the *exact order* of operations inside the thread. For example:

```cpp
    A = 1;
    B = 2;
    C = 3;
    ready.store(true, std::memory_order_release);
```

The compiler/CPU might internally reorder A, B, and C as C, B, A, but it will **always** ensure that all of them are completed **before** `ready` is visible to other threads.

### 3.5 Partial Data Synchronization

`memory_order_release` and `memory_order_acquire` affect **all memory operations** in the current thread relative to the atomic. You cannot directly limit them to just a subset of variables. If you need fine-grained control, you can:

* Use separate atomics for different data blocks.
* Use a `std::atomic_thread_fence` to insert memory barriers around specific operations.
* Or fall back to `std::mutex` if correctness requires structured locking.

### 3.6 Example of Release/Acquire

```cpp
    std::atomic<bool> ready = false;
    int A, B, C;

    void producer() {
        A = 1;
        B = 2;
        C = 3;
        ready.store(true, std::memory_order_release);
    }

    void consumer() {
        while (!ready.load(std::memory_order_acquire)) {
            // spin-wait
        }
        // At this point, we are guaranteed to see A=1, B=2, C=3
    }
```

### 3.7 Quick Reference Table

| Memory Order | Guarantees (for `store`) | Guarantees (for `load`) | Use Case |
| :--- | :--- | :--- | :--- |
| `relaxed` | Atomicity only | Atomicity only | Performance-critical counters |
| `release` | All prior writes are visible before this store | - | Publish data + set flag |
| `acquire` | - | No later reads/writes move before this load | Wait for flag before reading data |
| `acq_rel` | Both acquire and release semantics | Both acquire and release semantics | Read-modify-write (e.g., `fetch_add`) |
| `seq_cst` | Global ordering of all atomics (strongest) | Global ordering of all atomics | Easier reasoning, less performance |

### 3.8 Visual Model

You can think of `release` as putting a **barrier behind all previous operations**, while `acquire` puts a **barrier in front of all subsequent operations**.

```cpp
    Thread A:                 Thread B:
    A = 1;                    while (!ready.load(acquire)) {}
    B = 2;                    read A, B safely
    ready.store(release);
```

### 3.9 If You Need Strict Ordering of A, B, C

`memory_order_release` guarantees that A, B, C will complete **before** `ready`, but not their order among themselves. If the order A->B->C is crucial, you need explicit barriers or use `seq_cst`:

```cpp
    A = 1;
    std::atomic_thread_fence(std::memory_order_seq_cst);
    B = 2;
    std::atomic_thread_fence(std::memory_order_seq_cst);
    C = 3;
    ready.store(true, std::memory_order_seq_cst);
```

### 3.10 Key Takeaways

* Use `release` on the producer side and `acquire` on the consumer side for efficient synchronization.
* `seq_cst` is simpler but may be slower.
* Memory orderings affect **all** preceding/following operations relative to the atomic, not just one variable.
* When in doubt, start with `seq_cst`, then optimize.

**Would you like me to create a short “cheat sheet” PDF with diagrams (release/acquire arrows, happens-before graph) that you could give to your student?**

### 3.11 Exercises

* Implement a simple spinlock using `std::atomic_flag`.
* Experiment with `memory_order_relaxed` vs `memory_order_acquire`.

---

## 4. Summary

* **Cache-friendly data structures:** Keep data contiguous and avoid pointer chasing.
* **Inline assembly:** Use rarely and prefer intrinsics.
* **Memory ordering:** Understand `std::atomic` and use the correct ordering semantics.

**Next Step:** Apply these concepts in small benchmarks and observe performance differences.
