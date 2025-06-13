#ifndef INFRA_ILOGGER_H
#define INFRA_ILOGGER_H

#include <string>

namespace infra {

// ILogger is an interface for logging messages.
// This abstract class defines the contract for any logging implementation.
class ILogger {
public:
    // Virtual destructor is crucial for polymorphic base classes.
    virtual ~ILogger() = default;

    // Logs an informational message.
    virtual void log_info(const std::string& message) = 0;

    // Logs an error message.
    virtual void log_error(const std::string& message) = 0;
};

} // namespace infra

#endif // INFRA_ILOGGER_H