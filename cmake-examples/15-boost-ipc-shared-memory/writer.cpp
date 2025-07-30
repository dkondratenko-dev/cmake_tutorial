#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

using namespace boost::interprocess;

int main() {
    try {
        // Remove any existing shared memory
        shared_memory_object::remove("SharedMemoryExample");
        
        // Create shared memory object
        shared_memory_object shm(create_only, "SharedMemoryExample", read_write);
        
        // Set size
        shm.truncate(1024);
        
        // Map the whole shared memory in this process
        mapped_region region(shm, read_write);
        
        // Get the address of the mapped region
        void* addr = region.get_address();
        
        // Write a message
        const char* message = "Hello from Writer Process!";
        std::strcpy(static_cast<char*>(addr), message);
        
        std::cout << "Writer: Message written to shared memory: " << message << std::endl;
        std::cout << "Writer: Press Enter to cleanup and exit..." << std::endl;
        std::cin.get();
        
        // Cleanup
        shared_memory_object::remove("SharedMemoryExample");
        
    } catch (const std::exception& e) {
        std::cerr << "Writer error: " << e.what() << std::endl;
        shared_memory_object::remove("SharedMemoryExample");
        return 1;
    }
    
    return 0;
}
