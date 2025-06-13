#include "user_processor.h" // Include the header for UserProcessor
#include <iostream>         // For demonstration (not strictly needed for actual logging logic)

namespace app {

// Constructor: Initializes the UserProcessor with the provided logger.
// The unique_ptr ensures proper ownership and cleanup of the logger.
UserProcessor::UserProcessor(std::unique_ptr<infra::ILogger> logger)
    : logger_(std::move(logger)) { // Use std::move for unique_ptr transfer
    if (!logger_) {
        // Log an error if the logger is null, though it shouldn't happen with proper usage.
        // In a real app, you might throw an exception or use a fallback logger.
        std::cerr << "UserProcessor: Provided logger is null!" << std::endl;
    }
}

void UserProcessor::process_user(const std::string& username) {
    // Simulate some processing logic
    std::string message = "Processing user: " + username;
    logger_->log_info(message); // Use the injected logger to log info
    // ... further processing ...
}

bool UserProcessor::delete_user(const std::string& username) {
    // Simulate user deletion logic. For demonstration, we'll fail for "admin".
    if (username == "admin") {
        std::string error_msg = "Attempted to delete protected user: " + username;
        logger_->log_error(error_msg); // Use the injected logger to log error
        return false; // Deletion failed
    } else {
        std::string info_msg = "User deleted successfully: " + username;
        logger_->log_info(info_msg); // Use the injected logger to log info
        return true; // Deletion successful
    }
}

} // namespace app