#ifndef APP_USER_PROCESSOR_H
#define APP_USER_PROCESSOR_H

#include <string>
#include <memory> // For std::unique_ptr
#include "infra/ilogger.h" // Include the ILogger interface

namespace app {

// UserProcessor handles operations related to users.
// It depends on an ILogger instance for logging.
class UserProcessor {
private:
    // Using a unique_ptr to manage the logger's lifetime.
    // This allows for flexible dependency injection (e.g., real or mock logger).
    std::unique_ptr<infra::ILogger> logger_;
    // infra::ILogger*;

public:
    // Constructor uses dependency injection: an ILogger instance is passed in.
    explicit UserProcessor(std::unique_ptr<infra::ILogger> logger);

    // Processes a user. Logs an info message.
    void process_user(const std::string& username);

    // Deletes a user. Logs an info message if successful, error if not found.
    bool delete_user(const std::string& username);
};

} // namespace app

#endif // APP_USER_PROCESSOR_H