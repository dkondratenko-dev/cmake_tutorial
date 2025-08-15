#include "DataProcessor.h"
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>

DataProcessor::DataProcessor() : processedCount(0) {}

void DataProcessor::processOnce() {
    data.push_back(processedCount);
    if (processedCount % 10 == 0) {
        std::cout << "Processed " << processedCount 
                  << " items, vector size: " << data.size() << std::endl;
    }
    processedCount++;
    if (data.size() > 1000) {
        data.clear();
        std::cout << "Cleared data buffer" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int DataProcessor::getProcessedCount() const {
    return processedCount;
}

size_t DataProcessor::getDataSize() const {
    return data.size();
}
