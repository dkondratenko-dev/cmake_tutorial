#include <iostream>
#include <x86intrin.h>
#include <cstdint> 

/**
 * __rdtsc(), __rdtscp() - return unsigned 64-bit value of the current CPU TSC - Time Stamp Counter
 */

inline uint64_t rdtsc_start() {
    // cpuid guarantees serialization - all instructions are finished before executing cpuid
    // and no new instructions are executed.
    // It is important to use cpuid before rdtscp, otherwise __rdtsc() can be reordered by the compiler or CPU
    unsigned int dummy;
    __asm__ __volatile__("cpuid" : "=a"(dummy) : "a"(0) : "ebx", "ecx", "edx");
    return __rdtsc();
}

inline uint64_t rdtsc_end() {
    // __rdtscp - newer version of rdtsc available with  Intel Nehalem / AMD K10.
    // __rdtscp guarantees serialization - all instructions are finished before executing __rdtscp
    // cpuid is needed to prevent instructions execution after __rdtscp before __rdtscp is finished
    // __rdtscp only guarantees that all instuctions will be finsihed before it execution, but it not guarantee that no instructions will called after it
    unsigned int dummy;
    uint64_t r = __rdtscp(&dummy);
    __asm__ __volatile__("cpuid" : "=a"(dummy) : "a"(0) : "ebx", "ecx", "edx");
    return r;
}

void func_to_benchmark() {
    volatile int x = 0;
    for (int i = 0; i < 1000; ++i)
        x += i;
}

int main() {
    uint64_t start = rdtsc_start();

    func_to_benchmark();

    uint64_t end = rdtsc_end();

    std::cout << "CPU cycles: " << (end - start) << std::endl;
    return 0;
}
