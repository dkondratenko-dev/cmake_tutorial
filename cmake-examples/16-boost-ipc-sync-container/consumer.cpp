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
        // Wait for producer to create shared memory
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
        // Open existing shared memory object
        shared_memory_object shm(open_only, "SyncExample", read_write);
        
        // Map the shared memory
        mapped_region region(shm, read_write);
        
        // Get pointer to shared data
        SharedData* data = static_cast<SharedData*>(region.get_address());
        
        std::cout << "Consumer: Starting to consume data..." << std::endl;
        
        int last_counter = 0;
        
        // Consume until producer is finished
        while (true) {
            {
                scoped_lock<interprocess_mutex> lock(data->mutex);
                
                // Check if there's new data
                if (data->counter > last_counter) {
                    std::cout << "Consumer: Consumed item " << data->counter 
                              << " - " << data->message << std::endl;
                    last_counter = data->counter;
                }
                
                // Check if producer is finished
                if (data->finished) {
                    std::cout << "Consumer: Producer finished. Exiting." << std::endl;
                    break;
                }
            }
            
            // Wait a bit before checking again
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Consumer error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
