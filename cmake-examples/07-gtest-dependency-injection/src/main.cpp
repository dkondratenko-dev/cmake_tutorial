#include <iostream>
#include <memory>       // For std::make_unique
#include "app/user_processor.h" // Your application logic
#include "infra/real_logger.h"  // Your real logger implementation

int main() {
    std::cout << "Starting application..." << std::endl;

    // Create a real logger instance
    auto real_logger = std::make_unique<infra::RealLogger>();

    // Inject the real logger into the UserProcessor
    app::UserProcessor processor(std::move(real_logger));

    // Use the user processor
    processor.process_user("Alice");
    processor.process_user("Bob");

    // Attempt to delete users
    if (processor.delete_user("Bob")) {
        std::cout << "Main: Bob was deleted." << std::endl;
    } else {
        std::cout << "Main: Failed to delete Bob." << std::endl;
    }

    if (processor.delete_user("admin")) {
        std::cout << "Main: Admin was deleted." << std::endl;
    } else {
        std::cout << "Main: Failed to delete admin (as expected)." << std::endl;
    }

    std::cout << "Application finished." << std::endl;
    return 0;
}