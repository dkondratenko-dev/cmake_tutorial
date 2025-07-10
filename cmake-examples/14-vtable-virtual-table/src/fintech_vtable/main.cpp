#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

#include "TradableAsset.h"
#include "fintech_vtable/stock/Stock.h" 
#include "fintech_vtable/bond/Bond.h"

// This function works with any TradableAsset, thanks to polymorphism.
void print_portfolio_summary(const std::vector<std::unique_ptr<TradableAsset>>& portfolio) {
    double totalValue = 0.0;
    std::cout << "\n--- Portfolio Summary ---\n";
    std::cout << std::fixed << std::setprecision(2);

    for (const auto& asset : portfolio) {
        // Dynamic dispatch happens here!
        // asset->vptr is used to find the V-Table, which points to the correct
        // implementation of getCurrentValue() (either Stock's or Bond's).
        double value = asset->getCurrentValue();
        std::cout << "Asset ID: " << asset->getId()
                  << ", Current Value: $" << value << std::endl;
        totalValue += value;
    }
    std::cout << "--------------------------------\n";
    std::cout << "Total Portfolio Value: $" << totalValue << std::endl;
    std::cout << "--------------------------------\n";
}

int main() {
    // Create a portfolio of different asset types.
    std::vector<std::unique_ptr<TradableAsset>> portfolio;

    portfolio.push_back(std::make_unique<Stock>("STK001", "AAPL", 150, 175.50));
    portfolio.push_back(std::make_unique<Bond>("BND001", 10000.00, 0.05));
    portfolio.push_back(std::make_unique<Stock>("STK002", "GOOG", 50, 130.25));
    
    // Process the entire portfolio uniformly.
    print_portfolio_summary(portfolio);
    
    // The unique_ptr will automatically call the virtual destructors in the correct order
    // when `portfolio` goes out of scope.

    return 0;
}