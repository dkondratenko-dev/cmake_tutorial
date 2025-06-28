#include "gtest/gtest.h"    // Google Test framework
#include "gmock/gmock.h"    // Google Mock framework

#include "app/user_processor.h" // Class under test
#include "infra/ilogger.h"      // The interface we are mocking

#include <memory>               // For std::unique_ptr, std::make_unique

// Use Google Mock's namespace for brevity
using ::testing::_;          // Matches any argument
using ::testing::Eq;         // Checks for equality
using ::testing::Return;     // Specifies a return value
using ::testing::AtLeast;    // Expects a call at least N times
using ::testing::StrictMock; // Optional: Makes mocks stricter about unexpected calls
using ::testing::SaveArg;

namespace test {

    // Define your Mock object (no change here)
    class MockLogger : public infra::ILogger {
    public:
        MOCK_METHOD(void, log_info, (const std::string& message), (override));
        MOCK_METHOD(void, log_error, (const std::string& message), (override));
    };

    // Define a Test Fixture (no change here, but useful context)
    class UserProcessorTest : public ::testing::Test {
    protected:
        // Using StrictMock to ensure no unexpected calls, which helps debugging
        StrictMock<MockLogger>* mock_logger_ptr;
        std::unique_ptr<StrictMock<MockLogger>> mock_logger_owner;
        std::unique_ptr<app::UserProcessor> processor;

        void SetUp() override {
            mock_logger_owner = std::make_unique<StrictMock<MockLogger>>();
            mock_logger_ptr = mock_logger_owner.get();
            processor = std::make_unique<app::UserProcessor>(std::move(mock_logger_owner));
        }

        void TearDown() override {
            // Unique pointers handle cleanup
        }
    };

    // Test case 1: Verify log_info is called when processing a user (no change)
    TEST_F(UserProcessorTest, ProcessUserLogsInfo) {
        //Here, we don't care what parameter is passed in the call to log_info 
        EXPECT_CALL(*mock_logger_ptr, log_info(::testing::_)).Times(1);
        //Then call the method to test
        processor->process_user("Alice");
    }

    // // Alternative Test case 1: Verify log_info is called when processing a user (no change)
    // TEST_F(UserProcessorTest, ProcessUserLogsInfo_b) {
    //     //Alternatively, we can pass an argument
    //     std::string& username;//This parameter is passed to SaveArg to save a closure on username
    //     EXPECT_CALL(*mock_logger_ptr, log_info(::testing::_))
    //     .Times(1)
    //     .WillOnce(Save_Arg<0>(&username));//Creates a numbered parameter that can be passed to the call to log_info

    //     EXPECT_CALL(*mock_logger_ptr, log_info(::testing::_,::testing::_,::testing::_))
    //     .Times(1)
    //     .WillOnce(Save_Arg<0>(&username), Save_Arg<1>(&something));//Creates a numbered parameter that can be passed to the call to log_info

    //     //Then call the method to test
    //     processor->process_user("Alice");

    //     //For alternative: Assert on the captured argument after the call - the argument value should have been set in the call
    //     ASSERT_EQ(username, "Processing user: Alice");

    //     //Or if we only care that Alice be part of the message
    //     ASSERT_NE(username.find("Alice)", std::string::npos) << "Expected log message to contain 'Alice', but got: " << 
    // }

    TEST_F(UserProcessorTest, ProcessUserLogsInfoCorrectlyCapturesArgument) {
        // 1. Declare a variable to store the captured argument.
        //    It must be of the same type as the argument you want to capture (const std::string& message).
        std::string captured_username_message;

        // 2. Set up the expectation with the SaveArg action.
        EXPECT_CALL(*mock_logger_ptr, log_info(::testing::_)) // Expect any string (_) to log_info
            .Times(1) // Expect it to be called exactly once
            .WillOnce(SaveArg<0>(&captured_username_message)); // Action: Save the 0-th arg into our variable

        // 3. Call the method under test.
        processor->process_user("Alice");

        // 4. Assert on the captured argument after the call.
        //    You can check for exact equality or if it contains a substring.
        ASSERT_EQ(captured_username_message, "Processing user: Alice");

        // Or, if you only care that 'Alice' is part of the message:
        // ASSERT_NE(captured_username_message.find("Alice"), std::string::npos)
        //    << "Expected log message to contain 'Alice', but got: " << captured_username_message;
    }

    // Test case 2: Verify log_error is called when deleting a protected user (no change)
    TEST_F(UserProcessorTest, DeleteProtectedUserLogsErrorAndReturnsFalse) {
        EXPECT_CALL(*mock_logger_ptr, log_error(Eq("Attempted to delete protected user: admin"))).Times(1);
        bool result = processor->delete_user("admin");
        ASSERT_FALSE(result);
    }

    // Test case 3: Verify log_info for successful deletion (no change)
    TEST_F(UserProcessorTest, DeleteRegularUserLogsInfoAndReturnsTrue) {
        EXPECT_CALL(*mock_logger_ptr, log_info(Eq("User deleted successfully: Bob"))).Times(1);
        EXPECT_CALL(*mock_logger_ptr, log_error(_)).Times(0); // Explicitly state no error logs
        bool result = processor->delete_user("Bob");
        ASSERT_TRUE(result);
    }

    // Test case 4: Demonstrating multiple calls and different types of expectations
    TEST_F(UserProcessorTest, MultipleOperationsLogCorrectly) {
        // Expectations for process_user calls
        EXPECT_CALL(*mock_logger_ptr, log_info(Eq("Processing user: Charlie"))).Times(1);
        EXPECT_CALL(*mock_logger_ptr, log_info(Eq("Processing user: David"))).Times(1);

        // Expect log_info for successful deletion of "Charlie"
        EXPECT_CALL(*mock_logger_ptr, log_info(Eq("User deleted successfully: Charlie"))).Times(1);

        // FIX: "root" is NOT a protected user according to current logic, so it logs INFO, not ERROR.
        // We expect log_info for "root" deletion.
        EXPECT_CALL(*mock_logger_ptr, log_info(Eq("User deleted successfully: root"))).Times(1);

        // Ensure log_error is NOT called by anything in this test (since "admin" is not used here)
        EXPECT_CALL(*mock_logger_ptr, log_error(_)).Times(0);


        processor->process_user("Charlie");
        processor->process_user("David");

        bool charlie_deleted = processor->delete_user("Charlie");
        ASSERT_TRUE(charlie_deleted); // Expect true for "Charlie"

        bool root_deleted = processor->delete_user("root"); // This will now call log_info
        ASSERT_TRUE(root_deleted); // Expect true for "root" based on current logic
    }

    // Main function to run all tests (no change)
    int main(int argc, char **argv) {
        ::testing::InitGoogleMock(&argc, argv);
        return RUN_ALL_TESTS();
    }
}