#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp> // Include condition
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstring> // For std::strcpy and sprintf

using namespace boost::interprocess;

struct SharedData {
    interprocess_mutex mutex;
    interprocess_condition cond_new_item;
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
        SharedData* data = new(region.get_address()) SharedData;//This is a Placement New - The syntax new (region.get_address()) T() works with pre-allocated memory instead of allocating new memory. So, the object of type T is constructed at the specified address, but no new memory is allocated.
        data->counter = 0;
        data->finished = false;
        std::strcpy(data->message, "Initial message");
        
        std::cout << "Producer: Starting to produce data..." << std::endl;
        
        // Produce 5 items
        for (int i = 1; i <= 5; ++i) {
            // Simulate some work *before* locking
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            {
                scoped_lock<interprocess_mutex> lock(data->mutex);
                data->counter = i;
                sprintf(data->message, "Message #%d from Producer", i);
                std::cout << "Producer: Produced item " << i << std::endl;

                // *** Notify one waiting consumer that a new item is ready ***
                lock.unlock();
                data->cond_new_item.notify_one();
            }
        }
        
        // Signal completion
        {
            scoped_lock<interprocess_mutex> lock(data->mutex);
            data->finished = true;
            std::cout << "Producer: Finished producing. Notifying consumer to exit." << std::endl;

            // *** Notify the consumer a final time so it can see the 'finished' flag ***
            lock.unlock();
            data->cond_new_item.notify_one();
        }

        std::cout << "Producer: Press Enter to cleanup..." << std::endl;
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
