#include "FinanceCalculator.h"
#include <cassert>
#include <cmath>

int main() {
    double value = calculate_compound_interest(1000, 0.05, 2);
    assert(std::abs(value - 1102.5) < 0.01);
    return 0;
}
