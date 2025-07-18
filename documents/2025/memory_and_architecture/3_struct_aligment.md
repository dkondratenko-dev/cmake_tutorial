# C++ Structural Alignment: Memory Layout and Optimization

## Lecture Overview
Today we'll explore how C++ organizes data in memory, why alignment matters, and how to optimize memory usage through proper structure design. Each example includes expected output and detailed explanations.

---

## 1. What is Structural Alignment?

### The Basic Concept
Imagine you're packing boxes in a truck. You can't just throw them anywhere - they need to fit properly for efficiency and safety. Similarly, the CPU prefers data to be placed at specific memory addresses for optimal performance.

**Structural alignment** is how the compiler arranges data members within a struct or class to satisfy hardware requirements and optimize performance.

### What is Memory Alignment?

**Memory alignment** refers to the requirement that data objects must be stored at memory addresses that are multiples of a specific number, known as their **alignment requirement**. Every data type in C++ (and at the hardware level) has such a requirement.

For example:
* **`char`** usually has an alignment requirement of **1 byte**.
* **`int`** (typically 4 bytes) usually requires **4-byte alignment**. This means an `int` must start at an address divisible by 4 (e.g., `0x1000`, `0x1004`, `0x1008`, but not `0x1001`).
* **`double`** (typically 8 bytes) usually requires **8-byte alignment**.
* **Structures and classes** have an alignment requirement equal to the largest alignment requirement among their members.

You can determine the alignment requirement of any type using the **`alignof(T)` operator**.

### Structural Alignment and Padding Explained

Even if a class is not empty and doesn't have virtual functions, its `sizeof` might be larger than the sum of its individual member sizes. This is due to **structural alignment and padding**.

  * **Alignment Requirements:** Processors often read data most efficiently when it's located at memory addresses that are multiples of its size (e.g., an `int` at an `address % 4 == 0`).
  * **Padding:** To ensure that each member within a `struct` or `class` is properly aligned, the compiler might insert **unused "padding" bytes** between members or at the end of the class.

**Example:**

```cpp
struct MyStruct {
    char c;    // 1 byte
    int i;     // 4 bytes
    char c2;   // 1 byte
};
// Sum of members: 1 + 4 + 1 = 6 bytes

// ... later in code
std::cout << "sizeof(MyStruct): " << sizeof(MyStruct) << " bytes" << std::endl;
```

On a typical system where `int` requires 4-byte alignment, `sizeof(MyStruct)` will likely be **12 bytes**, not 6.

**Why?**

```
Memory Layout (Conceptual):
+---+---+---+---+---+---+---+---+---+---+---+---+
| c | P | P | P | i | i | i | i | c2| P | P | P |
+---+---+---+---+---+---+---+---+---+---+---+---+
  ^           ^           ^
  0           4           8           12
  (c)         (i)         (c2)
  P = Padding byte
```

  * `char c` takes 1 byte.
  * To align `int i` to a 4-byte boundary, 3 padding bytes are inserted after `c`.
  * `int i` takes 4 bytes.
  * `char c2` takes 1 byte.
  * Finally, to ensure that if `MyStruct` is part of an array, the *next* element also starts correctly aligned, padding might be added at the end of the struct (3 bytes in this case, to make the total size a multiple of the largest alignment requirement, which is 4 bytes for `int`).

This padding ensures that all members are read efficiently by the CPU.

### Why is Alignment Necessary? (Why it's Important for the CPU)

Memory alignment is critically important for two main reasons:

#### Hardware Requirements:
* Some processor architectures (like ARM, MIPS) demand that data access for specific types occurs at aligned addresses. If you try to read an `int` from an unaligned address (e.g., `0x1001`), it can lead to a **hardware exception**, typically resulting in a program crash (e.g., a segmentation fault or bus error).
* x86/x64 architectures (most common in desktops and servers) generally permit unaligned access, but it leads to a **significant performance degradation**.

#### Performance and CPU Cache:
* CPUs don't read data from main memory (RAM) one byte at a time. Instead, they read larger chunks called **cache lines** (typically 64 bytes).
* When an object is **aligned**, it will most likely fit entirely into one, or a minimal number of, cache lines, and its start will coincide with the beginning of a cache line. This allows the CPU to efficiently load the data into the cache in a single operation.
* If an object is **unaligned**, it might span across cache line boundaries. For instance, an `int` could start at the end of one cache line and finish at the beginning of the next. This means the CPU would need to load *two* cache lines instead of one to read that single `int`, which is significantly slower. This is known as a **split access** or a **cache miss**.

### Why Does Alignment Matter?

```cpp
#include <iostream>

struct Simple {
    char a;      // 1 byte
    int b;       // 4 bytes
    char c;      // 1 byte
};

int main() {
    std::cout << "Size of Simple: " << sizeof(Simple) << " bytes" << std::endl;
    
    Simple s;
    std::cout << "Address of a: " << (void*)&s.a << std::endl;
    std::cout << "Address of b: " << (void*)&s.b << std::endl;
    std::cout << "Address of c: " << (void*)&s.c << std::endl;
    
    return 0;
}
```

**Expected Output:**
```
Size of Simple: 12 bytes
Address of a: 0x7fff5fbff6ac
Address of b: 0x7fff5fbff6b0
Address of c: 0x7fff5fbff6b4
```

**What This Shows Us:**
- The struct takes 12 bytes, not 6 (1+4+1) as you might expect
- Notice the addresses: `a` is at 6ac, `b` is at 6b0 (4 bytes later), `c` is at 6b4 (4 bytes after `b`)
- There are 3 bytes of padding after `a` to align `b` to a 4-byte boundary
- There are 3 bytes of padding after `c` to make the struct size a multiple of 4

**Why 12 bytes instead of 6?**
- The CPU prefers to read 4-byte integers at addresses divisible by 4
- This creates "padding" bytes between members for efficiency

---

## 2. Understanding Memory Layout

### Basic Data Type Alignment Requirements

```cpp
#include <iostream>

void show_basic_alignments() {
    std::cout << "Data Type Sizes and Alignments:" << std::endl;
    std::cout << "char:   size=" << sizeof(char)   << ", align=" << alignof(char)   << std::endl;
    std::cout << "short:  size=" << sizeof(short)  << ", align=" << alignof(short)  << std::endl;
    std::cout << "int:    size=" << sizeof(int)    << ", align=" << alignof(int)    << std::endl;
    std::cout << "long:   size=" << sizeof(long)   << ", align=" << alignof(long)   << std::endl;
    std::cout << "float:  size=" << sizeof(float)  << ", align=" << alignof(float)  << std::endl;
    std::cout << "double: size=" << sizeof(double) << ", align=" << alignof(double) << std::endl;
    std::cout << "void*:  size=" << sizeof(void*)  << ", align=" << alignof(void*)  << std::endl;
}
```

**Expected Output (64-bit system):**
```
Data Type Sizes and Alignments:
char:   size=1, align=1
short:  size=2, align=2
int:    size=4, align=4
long:   size=8, align=8
float:  size=4, align=4
double: size=8, align=8
void*:  size=8, align=8
```

**What This Shows Us:**
- Each type has a natural alignment requirement equal to its size
- `char` can be placed at any address (alignment 1)
- `int` must be placed at addresses divisible by 4
- `double` must be placed at addresses divisible by 8

### Memory Layout Visualization

```cpp
#include <iostream>
#include <iomanip>

struct BadLayout {
    char a;      // 1 byte
    int b;       // 4 bytes
    char c;      // 1 byte
    double d;    // 8 bytes
};

void print_memory_layout() {
    BadLayout obj;
    
    std::cout << "sizeof(BadLayout): " << sizeof(BadLayout) << " bytes" << std::endl;
    std::cout << std::hex << std::uppercase;
    std::cout << "Address of obj: " << &obj << std::endl;
    std::cout << "Address of a:   " << (void*)&obj.a << std::endl;
    std::cout << "Address of b:   " << (void*)&obj.b << std::endl;
    std::cout << "Address of c:   " << (void*)&obj.c << std::endl;
    std::cout << "Address of d:   " << (void*)&obj.d << std::endl;
    
    // Calculate offsets
    std::cout << std::dec << "\nOffsets from start:" << std::endl;
    std::cout << "a offset: " << (char*)&obj.a - (char*)&obj << std::endl;
    std::cout << "b offset: " << (char*)&obj.b - (char*)&obj << std::endl;
    std::cout << "c offset: " << (char*)&obj.c - (char*)&obj << std::endl;
    std::cout << "d offset: " << (char*)&obj.d - (char*)&obj << std::endl;
}
```

**Expected Output:**
```
sizeof(BadLayout): 24 bytes
Address of obj: 0x7FFF5FBFF690
Address of a:   0x7FFF5FBFF690
Address of b:   0x7FFF5FBFF694
Address of c:   0x7FFF5FBFF698
Address of d:   0x7FFF5FBFF6A0

Offsets from start:
a offset: 0
b offset: 4
c offset: 8
d offset: 16
```

**Memory Layout Visualization:**
```
Byte:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
Data: [a][P][P][P][b][b][b][b][c][P][P][P][P][P][P][P][d][d][d][d][d][d][d][d]
      ^           ^              ^                    ^
     a=0         b=4            c=8                 d=16
```

**What This Shows Us:**
- `a` at offset 0 (no padding needed)
- 3 bytes of padding before `b` to align it to offset 4 (divisible by 4)
- `c` at offset 8 right after `b`
- 7 bytes of padding before `d` to align it to offset 16 (divisible by 8)
- Total size is 24 bytes, but actual data is only 14 bytes!

---

## 3. Alignment Rules and Calculations

### Rule 1: Natural Alignment

```cpp
#include <iostream>

struct AlignmentExamples {
    char c;    // offset 0, no padding needed
    short s;   // needs 2-byte alignment
    int i;     // needs 4-byte alignment
    double d;  // needs 8-byte alignment
};

void show_alignment_rules() {
    AlignmentExamples obj;
    
    std::cout << "Struct size: " << sizeof(AlignmentExamples) << std::endl;
    std::cout << "Struct alignment: " << alignof(AlignmentExamples) << std::endl;
    
    std::cout << "\nMember offsets:" << std::endl;
    std::cout << "c: " << (char*)&obj.c - (char*)&obj << std::endl;
    std::cout << "s: " << (char*)&obj.s - (char*)&obj << std::endl;
    std::cout << "i: " << (char*)&obj.i - (char*)&obj << std::endl;
    std::cout << "d: " << (char*)&obj.d - (char*)&obj << std::endl;
}
```

**Expected Output:**
```
Struct size: 24
Struct alignment: 8

Member offsets:
c: 0
s: 2
i: 4
d: 8
```

**What This Shows Us:**
- `c` at offset 0 (char needs no special alignment)
- `s` at offset 2 (1 byte padding after `c` to align short to even address)
- `i` at offset 4 (right after short, already 4-byte aligned)
- `d` at offset 8 (right after int, already 8-byte aligned)
- Struct alignment is 8 (the largest member alignment)
- Total size is 16 bytes of data + 8 bytes at the end to make struct size multiple of 8

### Rule 2: Struct Alignment Determines Total Size

```cpp
#include <iostream>

struct Example1 {
    char a;    // 1 byte
    short b;   // 2 bytes
};

struct Example2 {
    char a;    // 1 byte
    int b;     // 4 bytes
};

struct Example3 {
    char a;    // 1 byte
    double b;  // 8 bytes
};

void compare_struct_sizes() {
    std::cout << "Example1 (char+short): size=" << sizeof(Example1) 
              << ", align=" << alignof(Example1) << std::endl;
    std::cout << "Example2 (char+int):   size=" << sizeof(Example2) 
              << ", align=" << alignof(Example2) << std::endl;
    std::cout << "Example3 (char+double): size=" << sizeof(Example3) 
              << ", align=" << alignof(Example3) << std::endl;
}
```

**Expected Output:**
```
Example1 (char+short): size=4, align=2
Example2 (char+int):   size=8, align=4
Example3 (char+double): size=16, align=8
```

**What This Shows Us:**
- Example1: 1+2=3 bytes data → 4 bytes total (1 byte padding at end)
- Example2: 1+4=5 bytes data → 8 bytes total (3 bytes padding at end)
- Example3: 1+8=9 bytes data → 16 bytes total (7 bytes padding at end)
- The struct size must be a multiple of its alignment requirement

---

## 4. Optimizing Struct Layout

### Bad vs Good Layout Comparison

```cpp
#include <iostream>

// BAD: Lots of wasted space
struct BadStudent {
    char name[20];      // 20 bytes
    double gpa;         // 8 bytes
    int id;             // 4 bytes
    char grade;         // 1 byte
    short year;         // 2 bytes
};

// GOOD: Minimized padding
struct GoodStudent {
    char name[20];      // 20 bytes
    double gpa;         // 8 bytes  
    int id;             // 4 bytes
    short year;         // 2 bytes
    char grade;         // 1 byte
};

void compare_layouts() {
    std::cout << "Bad layout:" << std::endl;
    std::cout << "  Size: " << sizeof(BadStudent) << " bytes" << std::endl;
    std::cout << "  Alignment: " << alignof(BadStudent) << std::endl;
    
    BadStudent bad;
    std::cout << "  name offset: " << (char*)&bad.name - (char*)&bad << std::endl;
    std::cout << "  gpa offset:  " << (char*)&bad.gpa - (char*)&bad << std::endl;
    std::cout << "  id offset:   " << (char*)&bad.id - (char*)&bad << std::endl;
    std::cout << "  grade offset:" << (char*)&bad.grade - (char*)&bad << std::endl;
    std::cout << "  year offset: " << (char*)&bad.year - (char*)&bad << std::endl;
    
    std::cout << "\nGood layout:" << std::endl;
    std::cout << "  Size: " << sizeof(GoodStudent) << " bytes" << std::endl;
    std::cout << "  Alignment: " << alignof(GoodStudent) << std::endl;
    
    GoodStudent good;
    std::cout << "  name offset: " << (char*)&good.name - (char*)&good << std::endl;
    std::cout << "  gpa offset:  " << (char*)&good.gpa - (char*)&good << std::endl;
    std::cout << "  id offset:   " << (char*)&good.id - (char*)&good << std::endl;
    std::cout << "  year offset: " << (char*)&good.year - (char*)&good << std::endl;
    std::cout << "  grade offset:" << (char*)&good.grade - (char*)&good << std::endl;
}
```

**Expected Output:**
```
Bad layout:
  Size: 48 bytes
  Alignment: 8
  name offset: 0
  gpa offset:  24
  id offset:   32
  grade offset:36
  year offset: 38

Good layout:
  Size: 40 bytes
  Alignment: 8
  name offset: 0
  gpa offset:  24
  id offset:   32
  year offset: 36
  grade offset:38
```

**What This Shows Us:**

**Bad Layout Memory Map:**
```
[name: 20 bytes][4 pad][gpa: 8][id: 4][grade: 1][pad][year: 2][pad: 9]
0               20    24      32     36        37  38       40   48
```

**Good Layout Memory Map:**
```
[name: 20 bytes][4 pad][gpa: 8][id: 4][year: 2][grade: 1][pad: 1]
0               20    24      32     36       38        39   40
```

- Bad layout wastes 13 bytes (27% overhead!)
- Good layout wastes only 5 bytes (12.5% overhead)
- Simply reordering members saves 8 bytes per object

### Optimal Ordering Strategy

```cpp
#include <iostream>

struct OptimalLayout {
    // 8-byte aligned members first
    double salary;      // 8 bytes, offset 0
    void* ptr;          // 8 bytes, offset 8
    long timestamp;     // 8 bytes, offset 16
    
    // 4-byte aligned members
    int id;             // 4 bytes, offset 24
    float rating;       // 4 bytes, offset 28
    
    // 2-byte aligned members
    short department;   // 2 bytes, offset 32
    
    // 1-byte aligned members last
    char status;        // 1 byte,  offset 34
    bool active;        // 1 byte,  offset 35
    
    // Minimal padding needed
};

void show_optimal_layout() {
    OptimalLayout obj;
    
    std::cout << "Optimal layout size: " << sizeof(OptimalLayout) << std::endl;
    std::cout << "Alignment: " << alignof(OptimalLayout) << std::endl;
    
    std::cout << "\nMember offsets:" << std::endl;
    std::cout << "salary:     " << (char*)&obj.salary - (char*)&obj << std::endl;
    std::cout << "ptr:        " << (char*)&obj.ptr - (char*)&obj << std::endl;
    std::cout << "timestamp:  " << (char*)&obj.timestamp - (char*)&obj << std::endl;
    std::cout << "id:         " << (char*)&obj.id - (char*)&obj << std::endl;
    std::cout << "rating:     " << (char*)&obj.rating - (char*)&obj << std::endl;
    std::cout << "department: " << (char*)&obj.department - (char*)&obj << std::endl;
    std::cout << "status:     " << (char*)&obj.status - (char*)&obj << std::endl;
    std::cout << "active:     " << (char*)&obj.active - (char*)&obj << std::endl;
}
```

**Expected Output:**
```
Optimal layout size: 40
Alignment: 8

Member offsets:
salary:     0
ptr:        8
timestamp:  16
id:         24
rating:     28
department: 32
status:     34
active:     35
```

**Memory Layout:**
```
[salary:8][ptr:8][timestamp:8][id:4][rating:4][dept:2][status:1][active:1][pad:4]
0         8      16           24    28        32      34        35         40
```

**What This Shows Us:**
- Perfect alignment for all 8-byte members (no padding between them)
- 4-byte members fit perfectly after 8-byte members
- 2-byte and 1-byte members fill the remaining space efficiently
- Only 4 bytes of padding at the end (10% overhead)

---

## 5. Controlling Alignment

### Using alignas Specifier

```cpp
#include <iostream>

struct NormalAlignment {
    char data[13];
    int value;
};

struct CustomAlignment {
    alignas(16) char data[13];  // Force 16-byte alignment
    int value;
};

void test_custom_alignment() {
    std::cout << "Normal alignment:" << std::endl;
    std::cout << "  Size: " << sizeof(NormalAlignment) << std::endl;
    std::cout << "  Alignment: " << alignof(NormalAlignment) << std::endl;
    
    std::cout << "\nCustom alignment:" << std::endl;
    std::cout << "  Size: " << sizeof(CustomAlignment) << std::endl;
    std::cout << "  Alignment: " << alignof(CustomAlignment) << std::endl;
    
    NormalAlignment normal;
    CustomAlignment custom;
    
    std::cout << "\nAddresses:" << std::endl;
    std::cout << "Normal:  " << &normal << std::endl;
    std::cout << "Custom:  " << &custom << std::endl;
    std::cout << "Custom % 16 = " << (uintptr_t)&custom % 16 << std::endl;
}
```

**Expected Output:**
```
Normal alignment:
  Size: 20
  Alignment: 4

Custom alignment:
  Size: 32
  Alignment: 16

Addresses:
Normal:  0x7fff5fbff6a0
Custom:  0x7fff5fbff680
Custom % 16 = 0
```

**What This Shows Us:**
- Normal struct: 13+4=17 bytes → 20 bytes (4-byte alignment)
- Custom struct: same data → 32 bytes (16-byte alignment requirement)
- Custom struct address ends in 0 (divisible by 16)
- `alignas` forces stricter alignment but increases size

### Packing Structs (Remove Padding)

```cpp
#include <iostream>

struct NormalStruct {
    char a;
    int b;
    char c;
};

#pragma pack(push, 1)
struct PackedStruct {
    char a;
    int b;
    char c;
};
#pragma pack(pop)

void demonstrate_packing() {
    std::cout << "Normal struct:" << std::endl;
    std::cout << "  Size: " << sizeof(NormalStruct) << std::endl;
    std::cout << "  Alignment: " << alignof(NormalStruct) << std::endl;
    
    NormalStruct normal;
    std::cout << "  a offset: " << (char*)&normal.a - (char*)&normal << std::endl;
    std::cout << "  b offset: " << (char*)&normal.b - (char*)&normal << std::endl;
    std::cout << "  c offset: " << (char*)&normal.c - (char*)&normal << std::endl;
    
    std::cout << "\nPacked struct:" << std::endl;
    std::cout << "  Size: " << sizeof(PackedStruct) << std::endl;
    std::cout << "  Alignment: " << alignof(PackedStruct) << std::endl;
    
    PackedStruct packed;
    std::cout << "  a offset: " << (char*)&packed.a - (char*)&packed << std::endl;
    std::cout << "  b offset: " << (char*)&packed.b - (char*)&packed << std::endl;
    std::cout << "  c offset: " << (char*)&packed.c - (char*)&packed << std::endl;
}
```

**Expected Output:**
```
Normal struct:
  Size: 12
  Alignment: 4
  a offset: 0
  b offset: 4
  c offset: 8

Packed struct:
  Size: 6
  Alignment: 1
  a offset: 0
  b offset: 1
  c offset: 5
```

**What This Shows Us:**

**Normal Layout:**
```
[a][pad][pad][pad][b][b][b][b][c][pad][pad][pad]
0  1    2    3    4  5  6  7  8  9   10   11
```

**Packed Layout:**
```
[a][b][b][b][b][c]
0  1  2  3  4  5
```

- Normal: 12 bytes with proper alignment
- Packed: 6 bytes with no padding
- **Warning**: Accessing `packed.b` might be slower because it's not 4-byte aligned!

---

## 6. Advanced Alignment Concepts

### Virtual Function Impact on Size

```cpp
#include <iostream>

struct WithoutVirtual {
    char data;
    int value;
};

struct WithVirtual {
    virtual void func() {}
    char data;
    int value;
};

struct MultipleVirtual {
    virtual void func1() {}
    virtual void func2() {}
    char data;
    int value;
};

void virtual_size_demo() {
    std::cout << "Without virtual functions:" << std::endl;
    std::cout << "  Size: " << sizeof(WithoutVirtual) << std::endl;
    std::cout << "  Alignment: " << alignof(WithoutVirtual) << std::endl;
    
    std::cout << "\nWith virtual function:" << std::endl;
    std::cout << "  Size: " << sizeof(WithVirtual) << std::endl;
    std::cout << "  Alignment: " << alignof(WithVirtual) << std::endl;
    
    std::cout << "\nWith multiple virtual functions:" << std::endl;
    std::cout << "  Size: " << sizeof(MultipleVirtual) << std::endl;
    std::cout << "  Alignment: " << alignof(MultipleVirtual) << std::endl;
    
    WithVirtual obj;
    std::cout << "\nVirtual object member offsets:" << std::endl;
    std::cout << "  data offset:  " << (char*)&obj.data - (char*)&obj << std::endl;
    std::cout << "  value offset: " << (char*)&obj.value - (char*)&obj << std::endl;
}
```

**Expected Output:**
```
Without virtual functions:
  Size: 8
  Alignment: 4

With virtual function:
  Size: 16
  Alignment: 8

With multiple virtual functions:
  Size: 16
  Alignment: 8

Virtual object member offsets:
  data offset:  8
  value offset: 12
```

**What This Shows Us:**

**Without Virtual Layout:**
```
[data][pad][pad][pad][value][value][value][value]
0     1    2    3    4      5      6      7
```

**With Virtual Layout:**
```
[vptr][vptr][vptr][vptr][vptr][vptr][vptr][vptr][data][pad][pad][pad][value][value][value][value]
0     1     2     3     4     5     6     7     8     9    10   11   12     13     14     15
```

- Virtual function adds 8-byte virtual table pointer (vptr) at the beginning
- This increases size from 8 to 16 bytes
- Multiple virtual functions use the same vptr (no additional size cost)
- Member offsets shift to accommodate the vptr

### Cache Line Alignment Example

```cpp
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

struct BadCounters {
    std::atomic<int> counter1;
    std::atomic<int> counter2;  // Same cache line as counter1!
};

struct GoodCounters {
    alignas(64) std::atomic<int> counter1;  // Own cache line
    alignas(64) std::atomic<int> counter2;  // Own cache line
};

void show_cache_alignment() {
    std::cout << "Bad counters structure:" << std::endl;
    std::cout << "  Size: " << sizeof(BadCounters) << std::endl;
    std::cout << "  Alignment: " << alignof(BadCounters) << std::endl;
    
    BadCounters bad;
    std::cout << "  counter1 address: " << &bad.counter1 << std::endl;
    std::cout << "  counter2 address: " << &bad.counter2 << std::endl;
    std::cout << "  Distance: " << (char*)&bad.counter2 - (char*)&bad.counter1 << " bytes" << std::endl;
    
    std::cout << "\nGood counters structure:" << std::endl;
    std::cout << "  Size: " << sizeof(GoodCounters) << std::endl;
    std::cout << "  Alignment: " << alignof(GoodCounters) << std::endl;
    
    GoodCounters good;
    std::cout << "  counter1 address: " << &good.counter1 << std::endl;
    std::cout << "  counter2 address: " << &good.counter2 << std::endl;
    std::cout << "  Distance: " << (char*)&good.counter2 - (char*)&good.counter1 << " bytes" << std::endl;
}
```

**Expected Output:**
```
Bad counters structure:
  Size: 8
  Alignment: 4
  counter1 address: 0x7fff5fbff6a0
  counter2 address: 0x7fff5fbff6a4
  Distance: 4 bytes

Good counters structure:
  Size: 128
  Alignment: 64
  counter1 address: 0x7fff5fbff640
  counter2 address: 0x7fff5fbff680
  Distance: 64 bytes
```

**What This Shows Us:**
- Bad layout: counters are 4 bytes apart (same 64-byte cache line)
- Good layout: counters are 64 bytes apart (separate cache lines)
- In multithreaded code, bad layout causes "false sharing" - updating one counter affects the other's performance
- Good layout prevents false sharing but uses more memory (128 vs 8 bytes)

---

## 7. Practical Performance Examples

### Array Access Patterns

```cpp
#include <iostream>
#include <chrono>
#include <vector>

struct BadLayout {
    char flag;     // 1 byte
    double value;  // 8 bytes (7 bytes padding before)
    char id;       // 1 byte (7 bytes padding after)
};  // Total: 24 bytes per element

struct GoodLayout {
    double value;  // 8 bytes
    char flag;     // 1 byte
    char id;       // 1 byte (6 bytes padding after)
};  // Total: 16 bytes per element

void performance_comparison() {
    const size_t COUNT = 1000000;
    
    std::vector<BadLayout> bad_data(COUNT);
    std::vector<GoodLayout> good_data(COUNT);
    
    std::cout << "Memory usage comparison:" << std::endl;
    std::cout << "Bad layout:  " << sizeof(BadLayout) << " bytes per element" << std::endl;
    std::cout << "Good layout: " << sizeof(GoodLayout) << " bytes per element" << std::endl;
    std::cout << "Total bad:   " << sizeof(BadLayout) * COUNT / 1024 << " KB" << std::endl;
    std::cout << "Total good:  " << sizeof(GoodLayout) * COUNT / 1024 << " KB" << std::endl;
    std::cout << "Memory saved: " << (sizeof(BadLayout) - sizeof(GoodLayout)) * COUNT / 1024 << " KB" << std::endl;
    
    // Initialize data
    for (size_t i = 0; i < COUNT; ++i) {
        bad_data[i] = {1, static_cast<double>(i), 1};
        good_data[i] = {static_cast<double>(i), 1, 1};
    }
    
    // Time bad layout access
    auto start = std::chrono::high_resolution_clock::now();
    double sum1 = 0;
    for (const auto& item : bad_data) {
        sum1 += item.value;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto bad_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Time good layout access
    start = std::chrono::high_resolution_clock::now();
    double sum2 = 0;
    for (const auto& item : good_data) {
        sum2 += item.value;
    }
    end = std::chrono::high_resolution_clock::now();
    auto good_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "\nPerformance comparison:" << std::endl;
    std::cout << "Bad layout time:  " << bad_time.count() << " microseconds" << std::endl;
    std::cout << "Good layout time: " << good_time.count() << " microseconds" << std::endl;
    std::cout << "Speedup: " << static_cast<double>(bad_time.count()) / good_time.count() << "x" << std::endl;
}
```

**Expected Output:**
```
Memory usage comparison:
Bad layout:  24 bytes per element
Good layout: 16 bytes per element
Total bad:   23437 KB
Total good:  15625 KB
Memory saved: 7812 KB

Performance comparison:
Bad layout time:  3420 microseconds
Good layout time: 2150 microseconds
Speedup: 1.59x
```

**What This Shows Us:**
- Bad layout wastes 33% more memory (24 vs 16 bytes per element)
- Better cache utilization with good layout leads to 59% performance improvement
- Accessing 1M elements shows measurable difference in real applications

---

Examples:


// memcmp does not works correctly because it also compares the padding (garbage bits) for unaligned structures
// but also for the aligned?
bool equal(S& s, S& s1)
{
    return memcmp(&s, &s1);
}