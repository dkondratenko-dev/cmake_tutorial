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

        // Acquire the lock. It will be unlocked while waiting.
        scoped_lock<interprocess_mutex> lock(data->mutex);

        while (true) {
            // *** Wait until the counter changes from what we last saw ***
            // The lambda is the predicate that checks if we should stop waiting.
            // It will check it before waiting, and each time it's notified.
            data->cond_new_item.wait(lock, [&]() {
                return data->counter != last_counter || data->finished;
            });

            // If we woke up because the producer is finished and there's no new data
            if (data->finished && data->counter == last_counter) {
                std::cout << "Consumer: Producer finished. Exiting." << std::endl;
                break;
            }

            // A new item is available, process it
            std::cout << "Consumer: Consumed item " << data->counter
                      << " - " << data->message << std::endl;
            last_counter = data->counter;

            // If we woke up and processed the last item, check if we're also finished
            if(data->finished) {
                 std::cout << "Consumer: Producer finished after last item. Exiting." << std::endl;
                 break;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Consumer error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
