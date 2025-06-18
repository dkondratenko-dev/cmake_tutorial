// main.cpp
#include <iostream>
#include <vector>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::seconds

const size_t ALLOCATION_SIZE_MB = 1; // Allocate 1 MB per iteration
const std::chrono::milliseconds ALLOCATION_INTERVAL_MS(100); // Wait 100 milliseconds

void allocate_1gb() {
    char* chunk = new char[ALLOCATION_SIZE_MB * 1024 * 1024* 1024];
}

int main() {
    std::cout << "Starting continuous memory leak example..." << std::endl;
    std::cout << "Open another terminal and run 'top' or 'htop' to observe memory usage." << std::endl;
    std::cout << "Look for this process's RES (Resident) or VIRT (Virtual) memory growing." << std::endl;
    std::cout << "Press Ctrl+C to terminate." << std::endl;

    allocate_1gb();

    int32_t cnt = 0;

    // run loop for 1 min
    // {
    //     constexpr int32_t minutes = 1;
    //     constexpr int32_t min2msec = 1000 * 60;
    //     constexpr int64_t multiplier = 100'000;
    //     while (cnt++ < minutes * min2msec * multiplier) {
    //         // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //     }
        
    //     return 0;
    // }
    
    // We store the pointers to prevent the compiler from optimizing away the allocations.
    // However, we never delete them, causing a leak.
    std::vector<char*> leaked_memory_chunks;
    long long total_allocated_mb = 0;

    try {
        while (cnt++ < 10) {
            // Allocate 1 MB
            char* chunk = new char[ALLOCATION_SIZE_MB * 1024 * 1024];

            // Fill a small part of the chunk to ensure the memory is actually touched
            // and thus more likely to be counted in RES by the OS, not just VIRT.
            for (size_t i = 0; i < ALLOCATION_SIZE_MB * 1024 * 1024; i += 4096) { // Touch every 4KB page
                chunk[i] = static_cast<char>(i % 256);
            }

            leaked_memory_chunks.push_back(chunk);
            total_allocated_mb += ALLOCATION_SIZE_MB;

            std::cout << "Allocated " << ALLOCATION_SIZE_MB << " MB. Total: "
                      << total_allocated_mb << " MB leaked so far." << std::endl;

            // Pause for a short period to allow 'top' to refresh and show the change
            std::this_thread::sleep_for(ALLOCATION_INTERVAL_MS);
        }
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        std::cerr << "Total leaked before crash: " << total_allocated_mb << " MB." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    }

    // In a real application, if these were meant to be freed, you'd iterate and delete.
    // For this example, we intentionally don't.
    // for (char* chunk : leaked_memory_chunks) {
    //     delete[] chunk;
    // }
    // leaked_memory_chunks.clear();

    return 0;
}