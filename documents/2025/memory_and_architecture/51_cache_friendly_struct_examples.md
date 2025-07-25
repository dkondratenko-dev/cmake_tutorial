# Cache-Friendly Data Structures Playground (C++)

**Goal:** Give you practical, experiment-ready C++ examples that you and your student can build, run, tweak, and measure to *feel* the difference cache locality makes.

Each section:

* **Scenario** – what we’re testing.
* **Code** – minimal but realistic.
* **Try This** – experiments to run.
* **Expected Trend** – what you should observe (roughly; depends on hardware, compiler, build flags).

> **Build tip:** Use `-O3 -march=native -DNDEBUG` for performance experiments. Run multiple times. Pin CPU core if possible.

---

## 0. Benchmark Harness Helpers

A simple timing helper using `std::chrono`, plus a `do_not_optimize()` trick to stop the compiler from removing work.

```cpp
#pragma once
#include <chrono>
#include <cstdint>
#include <iostream>
#include <type_traits>

#if defined(__GNUC__) || defined(__clang__)
  template <class T>
  inline void do_not_optimize_away(T const& value) {
      asm volatile("" : : "g"(&value) : "memory");
  }
#else
  template <class T>
  inline void do_not_optimize_away(T const& value) { (void)value; }
#endif

class Timer {
public:
    using clock = std::chrono::high_resolution_clock;
    Timer() : start_(clock::now()) {}
    void reset() { start_ = clock::now(); }
    double elapsed_ms() const {
        auto end = clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }
private:
    clock::time_point start_;
};
```

Usage:

```cpp
Timer t; /* work */ double ms = t.elapsed_ms();
std::cout << label << ": " << ms << " ms\n";
```

---

## 1. Vector vs Linked List: Summation

**Scenario:** Contiguous `std::vector<int>` vs pointer-chasing linked list.

```cpp
#include <vector>
#include <random>
#include <memory>
#include <atomic>

struct Node {
    int value;
    Node* next;
};

Node* make_list(std::size_t n) {
    Node* head = nullptr;
    for (std::size_t i = 0; i < n; ++i) {
        Node* node = new Node{static_cast<int>(i), head};
        head = node;
    }
    return head; // reverse order
}

long long sum_list(Node* head) {
    long long s = 0;
    for (Node* p = head; p; p = p->next) s += p->value;
    do_not_optimize_away(s);
    return s;
}

long long sum_vector(const std::vector<int>& v) {
    long long s = 0;
    for (int x : v) s += x;
    do_not_optimize_away(s);
    return s;
}

int main() {
    constexpr std::size_t N = 50'000'000; // adjust for RAM
    std::vector<int> v; v.reserve(N); for (std::size_t i=0;i<N;++i) v.push_back((int)i);
    Node* list = make_list(N);

    Timer t;
    auto sv = sum_vector(v);
    double v_ms = t.elapsed_ms();

    t.reset();
    auto sl = sum_list(list);
    double l_ms = t.elapsed_ms();

    std::cout << "vector sum=" << sv << " time=" << v_ms << " ms\n";
    std::cout << "list   sum=" << sl << " time=" << l_ms << " ms\n";
}
```

**Try This:**

* Vary `N`.
* Shuffle list insertion order to simulate random allocation.
* Build with/without optimizations.

**Expected Trend:** `std::vector` wins strongly when `N` large due to spatial locality and fewer pointer dereferences.

---

## 2. Arrray of Struct(AoS) vs Struct of Array(SoA): Updating Particle Positions

**Scenario:** Update only `x` coordinate for many particles. AoS wastes bandwidth fetching unused `y,z,v`.

```cpp
#include <vector>
#include <cstdint>
#include <iostream>

struct ParticleAoS {
    float x, y, z;
    float v;
};

struct ParticleSoA {
    std::vector<float> x, y, z, v;
};

void init_aos(std::vector<ParticleAoS>& p, std::size_t n) {
    p.resize(n);
    for (std::size_t i=0;i<n;++i) {
        p[i].x = p[i].y = p[i].z = (float)i;
        p[i].v = 1.0f;
    }
}

void init_soa(ParticleSoA& p, std::size_t n) {
    p.x.resize(n); p.y.resize(n); p.z.resize(n); p.v.resize(n,1.0f);
    for (std::size_t i=0;i<n;++i) {
        p.x[i] = p.y[i] = p.z[i] = (float)i;
    }
}

void update_aos(std::vector<ParticleAoS>& p) {
    for (auto& e : p) {
        e.x += e.v; // touches full struct cache line
    }
}

void update_soa(ParticleSoA& p) {
    for (std::size_t i=0;i<p.x.size();++i) {
        p.x[i] += p.v[i]; // streams through two arrays only
    }
}

int main(){
    constexpr std::size_t N=50'000'000; // adjust
    std::vector<ParticleAoS> pa; ParticleSoA ps;
    init_aos(pa,N); init_soa(ps,N);

    Timer t; update_aos(pa); double aos_ms=t.elapsed_ms();
    t.reset(); update_soa(ps); double soa_ms=t.elapsed_ms();

    std::cout << "AoS update: " << aos_ms << " ms\n";
    std::cout << "SoA update: " << soa_ms << " ms\n";
}
```

**Try This:**

* Update all fields (AoS may catch up).
* Measure memory footprint via `sizeof(ParticleAoS)*N` vs vector sizes.

**Expected Trend:** When touching only `x`, SoA should be faster & more cache-friendly.

---

## 3. Struct Padding & Hot/Cold Splitting

**Scenario:** Reduce wasted bytes + pull hot fields together.

```cpp
#include <vector>
#include <cstdint>
#include <iostream>
#include <new>

// Bloated: cold stats in same cache line as hot state
struct EntityBad {
    bool alive;            // 1 byte
    double health;         // 8 bytes, may force padding
    char name[24];         // rarely used
    uint64_t lastSeenTick; // cold
};

// Hot/cold split: frequently accessed data is compact
struct EntityHot {
    float health;  // fit in 4 bytes
    uint32_t id;   // 4 bytes
    bool alive;    // 1 byte; pad manually if needed
};

struct EntityCold {
    char name[24];
    uint64_t lastSeenTick;
};

struct Entity {
    EntityHot hot;
    EntityCold* cold; // separate allocation, touched rarely
};

void update(std::vector<Entity>& ents) {
    for (auto& e : ents) {
        if (!e.hot.alive) continue;
        e.hot.health -= 0.1f;
    }
}

int main(){
    std::cout << "sizeof(EntityBad)  = " << sizeof(EntityBad) << "\n";
    std::cout << "sizeof(EntityHot)  = " << sizeof(EntityHot) << "\n";
    std::cout << "sizeof(Entity)     = " << sizeof(Entity) << " (hot + ptr)\n";
}
```

**Try This:**

* Create big arrays of `EntityBad` vs `Entity` and run `update()` loops.
* Measure cycles per entity.

**Expected Trend:** Hot/cold split improves cache hit rate when cold data is rarely accessed.

---

## 4. Array-of-Struct-of-Arrays (AoSoA) / Blocked Layout

**Scenario:** Combine SIMD-friendly SoA with cache-friendly blocking.

```cpp
#include <array>
#include <vector>
#include <immintrin.h> // optional

constexpr std::size_t BLOCK = 16; // tune for cache line / SIMD width

struct BlockParticles {
    std::array<float,BLOCK> x, y, z, v;
};

struct ParticlesAoSoA {
    std::vector<BlockParticles> blocks;
    std::size_t size=0; // total count
};

void init(ParticlesAoSoA& P, std::size_t n){
    P.size=n;
    std::size_t nb=(n+BLOCK-1)/BLOCK;
    P.blocks.resize(nb);
    for(std::size_t b=0;b<nb;++b){
        for(std::size_t i=0;i<BLOCK;++i){
            std::size_t idx=b*BLOCK+i; if(idx>=n) break;
            P.blocks[b].x[i]=(float)idx;
            P.blocks[b].y[i]=(float)idx;
            P.blocks[b].z[i]=(float)idx;
            P.blocks[b].v[i]=1.0f;
        }
    }
}

void update(ParticlesAoSoA& P){
    for(auto& blk : P.blocks){
        for(std::size_t i=0;i<BLOCK;++i){
            blk.x[i]+=blk.v[i];
        }
    }
}
```

**Try This:** Compare with pure AoS and pure SoA. Vary `BLOCK` to see L1/L2 behavior.

**Expected Trend:** For SIMD loads/stores and prefetching, AoSoA can outperform both AoS and SoA depending on access pattern.

---

## 5. Row-major vs Column-major Traversal

**Scenario:** Touching a 2D matrix in storage order vs transposed access.

```cpp
#include <vector>
#include <iostream>

struct Matrix {
    std::size_t rows, cols;
    std::vector<float> data; // row-major
    Matrix(std::size_t r, std::size_t c):rows(r),cols(c),data(r*c,0.f){}
    float& operator()(std::size_t r,std::size_t c){return data[r*cols+c];}
    const float& operator()(std::size_t r,std::size_t c)const{return data[r*cols+c];}
};

void touch_row_major(Matrix& M){
    for(std::size_t r=0;r<M.rows;++r)
        for(std::size_t c=0;c<M.cols;++c)
            M(r,c)+=1.f;
}

void touch_col_major(Matrix& M){
    for(std::size_t c=0;c<M.cols;++c)
        for(std::size_t r=0;r<M.rows;++r)
            M(r,c)+=1.f;
}

int main(){
    constexpr std::size_t R=8000,C=8000; // ~244MB at 4 bytes each (adjust!)
    Matrix M(R,C);
    Timer t; touch_row_major(M); double rm=t.elapsed_ms();
    t.reset(); touch_col_major(M); double cm=t.elapsed_ms();
    std::cout << "Row-major:    " << rm << " ms\n";
    std::cout << "Column-major: " << cm << " ms\n"; // expect slower
}
```

**Try This:**

* Shrink matrix to fit L3 vs overflow RAM.
* Compare stride-1 vs large strides.

**Expected Trend:** Row-major traversal (storage order) wins. Column-major causes many cache misses.

---

## 6. Blocking / Tiling for Better Locality

**Scenario:** Improve column-style traversal using tiles that fit cache.

```cpp
void touch_tiled(Matrix& M, std::size_t tile) {
    for (std::size_t br = 0; br < M.rows; br += tile) {
        for (std::size_t bc = 0; bc < M.cols; bc += tile) {
            std::size_t rmax = std::min(br + tile, M.rows);
            std::size_t cmax = std::min(bc + tile, M.cols);
            for (std::size_t r = br; r < rmax; ++r)
                for (std::size_t c = bc; c < cmax; ++c)
                    M(r,c) += 1.f;
        }
    }
}
```

**Try This:**

* Compare `touch_row_major`, `touch_col_major`, `touch_tiled(M,64)` / 128 / 256.
* Plot performance vs tile size.

**Expected Trend:** Good tile sizes that fit L1/L2 dramatically reduce misses on column-like access.

---

## 7. Index-Based Indirection Instead of Pointers

**Scenario:** Replace pointer-linked nodes with index vectors for stable references + contiguous storage.

```cpp
struct NodePool {
    struct Node { int value; int next; }; // -1 == none
    std::vector<Node> nodes;
    int head = -1;
};

long long sum_indexed(const NodePool& p) {
    long long s = 0;
    for (int i = p.head; i != -1; i = p.nodes[i].next) {
        s += p.nodes[i].value; // contiguous fetch of node
    }
    do_not_optimize_away(s);
    return s;
}
```

**Try This:** Build random linked structures but store nodes in vector; compare to raw pointer list.

**Expected Trend:** Better prefetching & packing; fewer TLB misses.

---

## 8. Small-Object / Inline Storage Buffers

**Scenario:** Avoid heap churn for small sequences; keep data on stack or inline storage.

```cpp
#include <array>
#include <vector>
#include <cstring>

// Simple small-vector (demo; not production)
template <typename T, std::size_t N>
class SmallVector {
public:
    SmallVector() : sz_(0), heap_(nullptr) {}
    ~SmallVector(){ if(heap_) delete[] heap_; }

    void push_back(const T& x){
        if(sz_ < N){ inline_[sz_++] = x; }
        else {
            if(sz_ == N) grow();
            heap_[sz_++ - N] = x;
        }
    }
    std::size_t size() const { return sz_; }
    const T& operator[](std::size_t i) const {
        return (i < N) ? inline_[i] : heap_[i-N];
    }
private:
    void grow(){ heap_ = new T[cap_]; /* naive */ }
    std::size_t sz_;
    std::size_t cap_ = 1024; // demo only
    T* heap_;
    std::array<T,N> inline_{};
};
```

**Idea:** For small counts, all data lives in object (likely on stack or in-place), giving great locality.

---

## 9. Prefetching (Bonus Experiment)

Manual prefetch can help with known future accesses.

```cpp
#if defined(__GNUC__) || defined(__clang__)
  #define PREFETCH(addr) __builtin_prefetch((addr))
#else
  #define PREFETCH(addr) ((void)0)
#endif

void sum_with_prefetch(const std::vector<int>& v){
    long long s=0; std::size_t n=v.size();
    for(std::size_t i=0;i<n;++i){
        if(i+16<n) PREFETCH(&v[i+16]); // prefetch 16 ahead
        s+=v[i];
    }
    do_not_optimize_away(s);
}
```

**Try This:**

* Prefetch different distances.
* Compare to baseline sum.

**Expected Trend:** Helps mainly for large strides or cache-thrashing patterns.

---

## 10. Putting It All Together: Benchmark Driver Skeleton

You can assemble all experiments under a single `main()` with CLI args:

```cpp
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./bench <test> [N]\n";
        return 1;
    }
    std::string test = argv[1];
    std::size_t N = (argc>2)? std::stoull(argv[2]) : 10'000'000;

    if (test == "vector_vs_list") {
        // call section 1 code
    } else if (test == "aos_vs_soa") {
        // call section 2 code
    } else if (test == "row_vs_col") {
        // call section 5 code
    } else if (test == "tiled") {
        // call section 6 code
    }
}
```

---

## 11. Measurement Checklist

Before believing any benchmark:

* Build Release (`-O3`).
* Disable debug iterators (MSVC / libc++).
* Warm up caches (run once, discard).
* Repeat ≥5 times; report median.
* Pin process to 1 core (Linux: `taskset`).
* Close turbo/power mgmt if you need stable numbers.

---

## 12. Discussion Prompts for Your Student

1. Why does `std::vector` beat `std::list` even if complexity is both `O(N)`?
2. When would AoS outperform SoA?
3. How does padding in a struct waste cache lines?
4. Why do we tile large matrices?
5. When *not* to micro-optimize for cache?
