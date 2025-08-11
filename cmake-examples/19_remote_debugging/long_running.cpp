#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <unistd.h>

// A simple server simulation that we can attach to while running - see /documents/2025/vscode_in_linux/remote_debugging.md, Docker Container Debugging Method 2: Manual Docker with GDB Server
//Requires running GDB server on the remote server
//To get the PID of this application, at terminal enter 
//$pgrep long_running              -------------should give PID
//  #Example output: 484055
//Next we attach gdbserver to PID 12345 and listen on port 9999       
//$gdbserver --attach :9999 484055
//  Attached: pid = 484055
//  Listening on port 9999
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