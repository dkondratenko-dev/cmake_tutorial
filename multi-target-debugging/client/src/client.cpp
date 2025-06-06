#include <iostream>
#include <string>

// Function to perform a simple calculation
double calculateClientValue(double input) {
    double result = input / 3.0 - 1.5;
    std::cout << "Client: Performing calculation with input " << input << std::endl;
    return result;
}

int main() {
    std::cout << "Client application started." << std::endl;

    double value = 30.0;
    double calculated = calculateClientValue(value); // Set a breakpoint here
    std::cout << "Client: Calculated value is " << calculated << std::endl;

    std::cout << "Client application finished." << std::endl;
    return 0;
}