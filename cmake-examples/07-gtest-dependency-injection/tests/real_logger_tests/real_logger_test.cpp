#include "gtest/gtest.h"        // Google Test framework
#include "infra/real_logger.h"  // The class we are testing

#include <sstream>              // For capturing cout/cerr (optional but good for real tests)
#include <streambuf>            // For capturing cout/cerr (optional)
#include <iostream>             // For cout/cerr

namespace test {

// A simple test fixture for RealLogger.
// No special setup is needed here, but it's good practice.
class RealLoggerTest : public ::testing::Test {
protected:
    infra::RealLogger logger;

    // Optional: Set up stream capturing for more robust tests
    std::stringstream captured_cout;
    std::stringstream captured_cerr;
    std::streambuf* old_cout_buffer = nullptr;
    std::streambuf* old_cerr_buffer = nullptr;

    void SetUp() override {
        // Redirect cout and cerr to our stringstreams for capture
        old_cout_buffer = std::cout.rdbuf();
        std::cout.rdbuf(captured_cout.rdbuf());

        old_cerr_buffer = std::cerr.rdbuf();
        std::cerr.rdbuf(captured_cerr.rdbuf());
    }

    void TearDown() override {
        // Restore cout and cerr buffers
        std::cout.rdbuf(old_cout_buffer);
        std::cerr.rdbuf(old_cerr_buffer);
    }
};

// Test case for log_info method
TEST_F(RealLoggerTest, LogInfoPrintsCorrectly) {
    std::string message = "This is an info message.";
    logger.log_info(message);

    // Verify the captured output
    std::string expected_output = "[INFO] " + message + "\n";
    ASSERT_EQ(captured_cout.str(), expected_output);
    ASSERT_TRUE(captured_cerr.str().empty()); // No error output expected
}

// Test case for log_error method
TEST_F(RealLoggerTest, LogErrorPrintsCorrectly) {
    std::string message = "This is an error message.";
    logger.log_error(message);

    // Verify the captured output
    std::string expected_output = "[ERROR] " + message + "\n";
    ASSERT_EQ(captured_cerr.str(), expected_output);
    ASSERT_TRUE(captured_cout.str().empty()); // No info output expected
}

// Main function to run all tests in this executable
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); // Initialize Google Test
    return RUN_ALL_TESTS();                // Runs all defined tests
}

}
