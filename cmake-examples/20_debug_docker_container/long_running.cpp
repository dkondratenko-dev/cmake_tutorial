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
    int pid = 0;
    
public:
    void processData() {
        pid = getpid();
        std::cout << "DataProcessor started to process data with PID: " << pid << std::endl;
        
        while (true) {
            // Simulate some work
            std::cout << "@@@@@@@@@ 1\n";
            data.push_back(processedCount);
            std::cout << "@@@@@@@@@ 2\n";
            std::cout << processedCount << "Allocated memory, but forgot to free it.\n";

            // This is where interesting things happen
            if (processedCount % 10 == 0) {
                std::cout << "Processed " << processedCount << " items" << std::endl;
                std::cout << "Current vector size: " << data.size() << std::endl;
            }
            
            processedCount++;
            
            // Clear vector periodically to avoid memory growth
            std::cout << "@@@@@@@@@ 3\n";
            if (data.size() > 1000) {
                std::cout << "@@@@@@@@@ 4\n";
                data.clear();
                std::cout << "@@@@@@@@@ 5\n";
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