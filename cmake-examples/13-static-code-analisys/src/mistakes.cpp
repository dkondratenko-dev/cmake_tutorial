// mistakes.cpp

#include <iostream>
#include <vector>
#include <string>

// Function with a potential memory leak
void memoryLeakFunction() {
    int* data = new int[10]; // Allocate memory
    // No delete[] for 'data', leading to a memory leak
    std::cout << "Allocated memory, but forgot to free it.\n";
    // Accessing out of bounds (bugprone-unchecked-array-usage, arrayIndexOutOfBounds)
    data[10] = 5; // Accessing index 10 for an array of size 10 (valid indices 0-9)
}

// Function with an uninitialized variable (uninitializedVariable)
void uninitializedVariableFunction() {
    int x; // Uninitialized variable
    if (x > 0) { // Using uninitialized variable
        std::cout << "x is positive.\n";
    } else {
        std::cout << "x is not positive.\n";
    }
}

// Function with a null pointer dereference (nullPointer)
void nullPointerDereference(int* ptr) {
    if (ptr == nullptr) {
        // Missing return or error handling here
    }
    *ptr = 100; // Dereferencing a potentially null pointer without proper check
}

// Function with unused variable (unusedVariable)
void unusedVariableFunction() {
    int y = 10; // Variable 'y' is declared and initialized but never used
    std::cout << "This function has an unused variable.\n";
}

// Function with redundant check (redundantCondition)
void redundantCheck(int a) {
    if (a > 0) {
        if (a > 5) { // If a > 5, then a > 0 is always true
            std::cout << "a is greater than 5.\n";
        }
    }
}

// Function with a potential division by zero (divisionByZero)
void divisionByZero(int denominator) {
    int result = 10 / denominator; // Potential division by zero if denominator is 0
    std::cout << "Result of division: " << result << "\n";
}

// Function with a copy-paste error (duplicateExpression)
void copyPasteError() {
    int val1 = 5;
    int val2 = 10;
    int sum = val1 + val2;
    int product = val1 * val2; // Suppose this was intended to be val1 + val1 or something else
    std::cout << "Sum: " << sum << ", Product: " << product << "\n";
}

// Simple main function to call the problematic functions
void testMistakes() {
    std::cout << "Running functions with potential mistakes...\n";

    memoryLeakFunction();
    uninitializedVariableFunction();

    int* validPtr = new int(10);
    nullPointerDereference(validPtr);
    delete validPtr; // Clean up after validPtr usage

    // Intentionally cause a null dereference (cppcheck might warn about this path)
    // int* nullPtr = nullptr;
    // nullPointerDereference(nullPtr); // This will cause a runtime crash if executed

    unusedVariableFunction();
    redundantCheck(7);
    redundantCheck(3);

    divisionByZero(2);
    // divisionByZero(0); // Uncommenting this will cause a runtime crash

    copyPasteError();

    std::cout << "Finished running functions.\n";
    return 0;
}