#include "real_logger.h" // Include the header for RealLogger
#include <iostream>      // For console output

namespace infra {

void RealLogger::log_info(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
    // SQL
}

void RealLogger::log_error(const std::string& message) {
    std::cerr << "[ERROR] " << message << std::endl;
}

} // namespace infra
