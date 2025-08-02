#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

using namespace boost::interprocess;//Has library features like sdt::cout << boost::backtrace::backtrace(); //prints call stack

int main() {
    try {
        // Ensure a clean start by removing any leftover segment
        shared_memory_object::remove("SharedMemoryExample");

        // Create the shared memory object
        shared_memory_object shm(create_only, "SharedMemoryExample", read_write);//Tells OS kernel to create a named object (inside kernel) and allocate RAM memory to it. The "create only" option will throw an exception if it is already created
        
        // Set size
        shm.truncate(1024);

        // Map the memory
        mapped_region region(shm, read_write);//This tells kernel to map this area of RAM to the virtual memory of the process. Kernel monitors this virtual memory and prevents one process from accessing the virtual memory of another process
        
        // Get the address of the mapped region
        void* addr = region.get_address();

        // Write the message
        const char* message = "Hello from a synchronized Writer!";
        std::strcpy(static_cast<char*>(addr), message);
        std::cout << "Writer: Message written to shared memory." << std::endl;
        std::cout << "Writer: Waiting for reader to read and remove the segment..." << std::endl;

        // Keep the program alive, waiting for the reader to remove the segment.
        // We can check periodically if the shared memory still exists.
        while (true) {
            try {
                // Try to open the shared memory. If it throws an exception,
                // it means the reader has removed it.
                shared_memory_object test_shm(open_only, "SharedMemoryExample", read_only);
                // If we are here, it still exists. Sleep and check again.
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } catch (const interprocess_exception& e) {
                // The reader has removed the segment, so we can exit.
                std::cout << "Writer: Detected that reader has removed the segment. Exiting." << std::endl;
                break;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception caught! Writer error: " << e.what() << std::endl;
        // Clean up in case of an error during setup
        shared_memory_object::remove("SharedMemoryExample");
        return 1;
    }
    
    return 0;
}
