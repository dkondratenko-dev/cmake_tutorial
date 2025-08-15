#include "../src/mylib/DataProcessor.h"
#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "Process started with PID: " << getpid() << std::endl;
    std::cout << "Attach debugger with: gdbserver :9999 ./long_running" << std::endl;

    DataProcessor processor;
    while (true) {
        processor.processOnce();
    }
    return 0;
}
