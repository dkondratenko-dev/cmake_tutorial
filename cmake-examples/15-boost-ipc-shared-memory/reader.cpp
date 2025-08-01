#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace boost::interprocess;

int main() {
    try {
        // Give the writer a moment to create the shared memory.
        // This sleep is still useful to prevent the reader from starting
        // before the writer has even created the object.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Open existing shared memory object
        shared_memory_object shm(open_only, "SharedMemoryExample", read_only);
        
        // Map the whole shared memory in this process
        mapped_region region(shm, read_only);

        // Get the address of the mapped region and read the message
        const char* message = static_cast<const char*>(region.get_address());
        std::cout << "Reader: Message read from shared memory: " << message << std::endl;

        // *** THIS IS THE CRITICAL ADDITION ***
        // The reader now removes the shared memory object once it's done.
        std::cout << "Reader: Cleaning up shared memory..." << std::endl;
        shared_memory_object::remove("SharedMemoryExample");

    } catch (const std::exception& e) {
        std::cerr << "Reader error: " << e.what() << std::endl;
        // Also try to clean up on error, just in case
        shared_memory_object::remove("SharedMemoryExample");
        return 1;
    }

    return 0;
}
