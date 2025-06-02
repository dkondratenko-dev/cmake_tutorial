#include <iostream>
#include <string>

// Function to perform a simple calculation
int calculateServerValue(int input) {
    int result = input * 2 + 5;
    std::cout << "Server: Performing calculation with input " << input << std::endl;
    return result;
}

int main() {
    std::cout << "Server application started." << std::endl;

    int value = 10;
    int calculated = calculateServerValue(value); // Set a breakpoint here
    std::cout << "Server: Calculated value is " << calculated << std::endl;

    std::cout << "Server application finished." << std::endl;
    return 0;
}
