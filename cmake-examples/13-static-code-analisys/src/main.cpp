#include "FinanceCalculator.h"
#include <iostream>
#include "mistakes.h"

int main() {
    double result = calculate_compound_interest(1000.0, 0.05, 10);
    std::cout << "Compound interest after 10 years: $" << result << std::endl;

    testMistakes();

    return 0;
}
