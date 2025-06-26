#include "FinanceCalculator.h"
#include <cmath>

double calculate_compound_interest(double principal, double rate, int years) {
    return principal * std::pow(1.0 + rate, years);
}
