#ifndef __DataProcessor_h__
#define __DataProcessor_h__

#include <vector>
#include <string>

class DataProcessor {
private:
    std::vector<int> data;
    int processedCount;
public:
    DataProcessor();
    void processOnce(); // One iteration for easier testing
    int getProcessedCount() const;
    size_t getDataSize() const;
};

#endif
