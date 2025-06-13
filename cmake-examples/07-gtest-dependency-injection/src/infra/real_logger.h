#ifndef INFRA_REAL_LOGGER_H
#define INFRA_REAL_LOGGER_H

#include "ilogger.h" // Include the ILogger interface

namespace infra {

// RealLogger provides a concrete implementation of ILogger.
// It prints messages to the console (stdout/stderr).
class RealLogger : public ILogger {
public:
    // Logs an informational message to standard output.
    void log_info(const std::string& message) override;

    // Logs an error message to standard error.
    void log_error(const std::string& message) override;
};

} // namespace infra

#endif // INFRA_REAL_LOGGER_H