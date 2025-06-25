#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iostream>
#include <string>

// --- Public Interface ---

// Forward declaration for the ScopedBenchmark helper class
class ScopedBenchmark;

/**
 * @class Benchmark
 * @brief A manual-start/stop benchmarking tool.
 *
 * Example:
 *   Benchmark b("My Test");
 *   b.start();
 *   // code to measure
 *   b.end();
 *   std::cout << b.toString() << std::endl;
 */
class Benchmark {
public:
    explicit Benchmark(std::string name);

    void start();
    void end();

    // Returns a formatted string with all timing statistics.
    std::string toString() const;

private:
    friend class ScopedBenchmark; // Allow ScopedBenchmark to access private members

    std::string m_name;
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::nanoseconds m_duration;
    bool m_hasEnded;
};


#ifdef ENABLE_BENCHMARK
    // The macro creates a uniquely named ScopedBenchmark object that lives for the current scope.
    // The ##__LINE__ ensures you can have multiple benchmarks in the same function.
    #define benchmark(name) ScopedBenchmark bench_##__LINE__(name)
#else
    // If benchmarking is disabled, the macro expands to nothing, incurring zero overhead.
    #define benchmark(name)
#endif


// --- Implementation Detail (Helper Class for RAII) ---

/**
 * @class ScopedBenchmark
 * @brief An RAII-style benchmarking tool.
 *
 * It starts the timer on construction and stops it on destruction (when it goes out of scope),
 * automatically printing the results. Use the `benchmark("name")` macro for convenience.
 */
class ScopedBenchmark {
public:
    explicit ScopedBenchmark(std::string name) : m_benchmark(std::move(name)) {
        m_benchmark.start();
    }

    ~ScopedBenchmark() {
        m_benchmark.end();
        // Automatically print on destruction
        std::cout << m_benchmark.toString() << std::endl;
    }

private:
    Benchmark m_benchmark;
};

#endif // BENCHMARK_H