#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace boost::interprocess;

struct SharedData {
    interprocess_mutex mutex;
    int counter;
    bool finished;
    char message[256];
};

int main() {
    try {
        // Remove any existing shared memory
        shared_memory_object::remove("SyncExample");
        
        // Create shared memory object
        shared_memory_object shm(create_only, "SyncExample", read_write);
        
        // Set size
        shm.truncate(sizeof(SharedData));
        
        // Map the shared memory
        mapped_region region(shm, read_write);
        
        // Construct the shared data in the mapped region
        SharedData* data = new(region.get_address()) SharedData;
        data->counter = 0;
        data->finished = false;
        std::strcpy(data->message, "Initial message");
        
        std::cout << "Producer: Starting to produce data..." << std::endl;
        
        // Produce 5 items
        for (int i = 1; i <= 5; ++i) {
            // Lock the mutex
            scoped_lock<interprocess_mutex> lock(data->mutex);
            
            // Update shared data
            data->counter = i;
            sprintf(data->message, "Message #%d from Producer", i);
            
            std::cout << "Producer: Produced item " << i << std::endl;
            
            // Simulate some work
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
        // Signal completion
        {
            scoped_lock<interprocess_mutex> lock(data->mutex);
            data->finished = true;
            std::cout << "Producer: Finished producing. Press Enter to cleanup..." << std::endl;
        }
        
        std::cin.get();
        
        // Cleanup
        shared_memory_object::remove("SyncExample");
        
    } catch (const std::exception& e) {
        std::cerr << "Producer error: " << e.what() << std::endl;
        shared_memory_object::remove("SyncExample");
        return 1;
    }
    
    return 0;
}
