#include "gtest/gtest.h"    // Include the Google Test framework
#include "mylib/calculator.h" // Include the header for the class we are testing

// Define a test fixture if you need common setup/teardown for multiple tests
// For simple cases, you can use raw TEST() macros directly.
class CalculatorTest : public ::testing::Test {
protected:
    // You can set up shared resources here
    mylib::Calculator calc;

    // This method is called before each test in this fixture
    void SetUp() override {
        // Optional setup
    }

    // This method is called after each test in this fixture
    void TearDown() override {
        // Optional teardown
    }
};

// Test case for the 'add' method
TEST_F(CalculatorTest, AddsTwoNumbers) {
    // Arrange (setup test data)
    // The 'calc' object is already set up by the fixture.

    // Act (perform the action to test)
    int result = calc.add(5, 3);

    // Assert (verify the result)
    ASSERT_EQ(8, result); // Checks if 8 is equal to result
}

// Test case for the 'subtract' method
TEST_F(CalculatorTest, SubtractsTwoNumbers) {
    // Arrange
    // The 'calc' object is already set up by the fixture.

    // Act
    int result = calc.subtract(10, 4);

    // Assert
    ASSERT_EQ(6, result);
}

// Another test for addition, demonstrating different inputs
TEST_F(CalculatorTest, AddsZero) {
    ASSERT_EQ(5, calc.add(5, 0));
    ASSERT_EQ(0, calc.add(0, 0));
}

// Main function to run all tests
// This is typically provided by the GTest framework, but we include it for completeness.
// In a real project, GTest usually provides a main or you link against its main.
// If you are building multiple test executables, you might need a main in each.
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); // Initializes the Google Test framework
    return RUN_ALL_TESTS();                // Runs all defined tests
}