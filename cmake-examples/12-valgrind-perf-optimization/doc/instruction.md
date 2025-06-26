# Profiling with Valgrind

First, let's build the code.

```bash
mkdir build && cd build
# We MUST use Debug build type to get symbols for Valgrind
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

## Step 1: Check for Memory Errors with `memcheck`

Even though our goal is performance, a quick check for memory leaks is always a good first step.

```bash
valgrind --leak-check=full ./option_pricer_naive
```

**Expected Output:**
You will see a summary like this. Notice `total heap usage`. It shows a huge number of allocations, which hints at a performance problem even if there are no memory *leaks*.

```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 2,530,001 allocs, 2,530,001 frees, 81,160,032 bytes allocated
==12345==
==12345== All heap blocks were freed -- no leaks are possible
```
2.5 million allocations is our first major red flag!

## Step 2: Profile CPU and Cache with `callgrind`

Now, let's find where the program is spending its time.

```bash
valgrind --tool=callgrind ./option_pricer_naive
```
This runs the program much more slowly and generates a file named `callgrind.out.<pid>`.

## Step 3: Analyze the Results with `callgrind_annotate`

This tool reads the output file and annotates our source code with performance metrics. The most important metric is `Ir` (Instructions Read).

```bash
# Replace <pid> with the **actual process ID** from the output file name
callgrind_annotate callgrind.out.<pid> src/naive.cpp
```

**Example:**

```bash
callgrind_annotate callgrind.out.2753851 ../src/naive.cpp
```

If you are executing from the build directory, you should use relative path to the source file. For example, if the source file is located in the `src` directory, you should use:
../src/naive.cpp

**Annotated Output (Key Sections):**
You will see the entire source file, but with numbers next to each line. Focus on the lines with the highest `Ir` counts.

```
--------------------------------------------------------------------------------
-- User-annotated source: src/naive.cpp
--------------------------------------------------------------------------------
Ir

... (some lines with low counts)

        8,008,000  double generate_normal_random() {
        8,008,000      std::random_device rd;
       32,032,000      std::mt19937 gen(rd());
       32,032,000      std::normal_distribution<> d(0.0, 1.0);
    1,017,016,000      return d(gen);
        8,008,000  }

...

     10,001  double run_monte_carlo(const OptionData& data) {
...
     10,000      for (int i = 0; i < data.num_simulations; ++i) {
            // VERY BAD: Unnecessary heap allocation in a tight loop.
            // We only need the final price, not the whole path.
     10,000          std::vector<double> path;
     10,000          path.push_back(data.initial_price);
...
  2,520,000          for (int j = 0; j < data.num_steps; ++j) {
1,114,272,000              double epsilon = generate_normal_random(); // Inefficient call
  2,520,000              double next_price = path.back() * std::exp(drift + diffusion * epsilon);
 78,120,000              path.push_back(next_price);
            }
...
```

**Analysis of the Annotation:**
*   **`generate_normal_random()`:** The function itself and its contents have gigantic `Ir` counts (billions!). Creating the generator and distribution inside the function is clearly the main bottleneck.
*   **`path.push_back(next_price)`:** This line has a very high cost (78 million). This confirms our suspicion that repeated vector re-allocations and copies are expensive.

---

## 4. The "Optimized" Implementation (`src/optimized.cpp`)

Now we fix the issues identified by `callgrind`.

1.  **Create RNG Once:** The random number generator and distribution are created once and passed by reference to the simulation function.
2.  **Remove Heap Allocations:** We eliminate the `std::vector` and just track the `current_price` in a simple double.

```cpp
// src/optimized.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <numeric>
#include <chrono>

struct OptionData {
    double initial_price;
    double strike_price;
    double risk_free_rate;
    double volatility;
    double time_to_maturity;
    int num_simulations;
    int num_steps;
};

// GOOD: The core simulation now takes the generator by reference.
// No new objects are created inside.
double run_monte_carlo(const OptionData& data, std::mt19937& gen, std::normal_distribution<>& dist) {
    double total_payoff = 0.0;
    double dt = data.time_to_maturity / data.num_steps;
    double drift = (data.risk_free_rate - 0.5 * data.volatility * data.volatility) * dt;
    double diffusion = data.volatility * std::sqrt(dt);

    for (int i = 0; i < data.num_simulations; ++i) {
        // GOOD: No more vector. Just track the current price.
        double current_price = data.initial_price;

        for (int j = 0; j < data.num_steps; ++j) {
            double epsilon = dist(gen); // EFFICIENT: just use the existing generator
            current_price *= std::exp(drift + diffusion * epsilon);
        }

        double payoff = std::max(current_price - data.strike_price, 0.0);
        total_payoff += payoff;
    }

    double mean_payoff = total_payoff / data.num_simulations;
    double discounted_price = mean_payoff * std::exp(-data.risk_free_rate * data.time_to_maturity);
    return discounted_price;
}

int main() {
    OptionData data;
    data.initial_price = 100.0;
    data.strike_price = 105.0;
    data.risk_free_rate = 0.05;
    data.volatility = 0.20;
    data.time_to_maturity = 1.0;
    data.num_simulations = 10000;
    data.num_steps = 252;

    // GOOD: Create the random number generator and distribution ONCE.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0, 1.0);

    auto start = std::chrono::high_resolution_clock::now();
    double price = run_monte_carlo(data, gen, dist); // Pass them in
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Optimized Implementation" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "Calculated Option Price: " << price << std::endl;
    std::cout << "Execution Time: " << duration.count() << " ms" << std::endl;

    return 0;
}
```

---

## 5. Verification and Final Benchmark

First, rebuild the code (if you've added the new file).
`make`

Now, let's run `callgrind` on the optimized version to see the difference.
```bash
valgrind --tool=callgrind ./option_pricer_optimized
# Note the new PID
callgrind_annotate callgrind.out.<new_pid> src/optimized.cpp
```
The annotated output for `optimized.cpp` will now show that the `Ir` counts inside the loop are dramatically lower, and the cost has shifted to the actual mathematical operations (`std::exp`), which is exactly what we expect. The expensive setup costs are gone.

### Final Performance Comparison

For a real-world benchmark, let's build in `Release` mode (which enables compiler optimizations like `-O3`) and compare execution times.

```bash
# In build directory
cmake .. -DCMAKE_BUILD_TYPE=Release
make

# Run the naive version
time ./option_pricer_naive

# Run the optimized version
time ./option_pricer_optimized
```

**Example Results on a typical machine:**

```
$ time ./option_pricer_naive
Naive Implementation
--------------------
Calculated Option Price: 8.025
Execution Time: 1350.12 ms

real    0m1.352s
user    0m1.352s
sys     0m0.000s

$ time ./option_pricer_optimized
Optimized Implementation
----------------------
Calculated Option Price: 8.019
Execution Time: 45.33 ms

real    0m0.047s
user    0m0.047s
sys     0m0.000s
```

The optimized version is nearly **30 times faster**! This demonstrates the immense power of using a profiler like Valgrind's Callgrind to identify and eliminate critical performance bottlenecks.