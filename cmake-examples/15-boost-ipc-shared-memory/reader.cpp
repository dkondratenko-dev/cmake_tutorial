#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace boost::interprocess;

int main() {
    try {
        // Wait a bit for writer to create shared memory
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Open existing shared memory object
        shared_memory_object shm(open_only, "SharedMemoryExample", read_only);
        
        // Map the whole shared memory in this process
        mapped_region region(shm, read_only);
        
        // Get the address of the mapped region
        void* addr = region.get_address();
        
        // Read the message
        const char* message = static_cast<const char*>(addr);
        
        std::cout << "Reader: Message read from shared memory: " << message << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Reader error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
