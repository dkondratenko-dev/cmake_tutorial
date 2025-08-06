#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <unistd.h>

// A simple server simulation that we can attach to while running
class DataProcessor {
private:
    std::vector<int> data;
    int processedCount = 0;
    
public:
    void processData() {
        while (true) {
            // Simulate some work
            data.push_back(processedCount);
            
            // This is where interesting things happen
            if (processedCount % 10 == 0) {
                std::cout << "Processed " << processedCount << " items" << std::endl;
                std::cout << "Current vector size: " << data.size() << std::endl;
            }
            
            processedCount++;
            
            // Clear vector periodically to avoid memory growth
            if (data.size() > 1000) {
                data.clear();
                std::cout << "Cleared data buffer" << std::endl;
            }
            
            // Sleep to simulate real work and avoid CPU spinning
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main() {
    std::cout << "Process started with PID: " << getpid() << std::endl;
    std::cout << "You can now attach a debugger to this process" << std::endl;
    
    DataProcessor processor;
    processor.processData();  // This runs forever
    
    return 0;
}