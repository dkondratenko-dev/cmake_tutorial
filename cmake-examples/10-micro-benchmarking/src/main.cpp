#include "benchmark/Benchmark.h"
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <thread> // For std::this_thread::sleep_for

// --- Simple Fintech Simulation ---

struct Order {
    int id;
    double price;
    int quantity;
    enum Side { BUY, SELL } side;
};

// A dummy function to simulate complex risk calculation
double calculatePortfolioRisk(const std::vector<Order>& orders) {
    // Simulate a CPU-intensive calculation
    double totalValue = 0.0;
    for (const auto& order : orders) {
        totalValue += order.price * order.quantity;
    }

    // Simulate some more work, like calculating volatility or VaR
    double risk = 0.0;
    for (size_t i = 0; i < orders.size(); ++i) {
        risk += std::log(orders[i].price) * std::sqrt(totalValue);
    }

    // Pretend this calculation takes some time
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    return risk / totalValue;
}

int main() {
    std::cout << "Starting Fintech Simulation...\n\n";

    std::vector<Order> orders;
    orders.reserve(50000);

    // --- 1. RAII-style (scoped) benchmark ---
    // The benchmark starts here and will automatically stop and print
    // when the scope (the curly braces) ends.
    {
        benchmark("Order Batch Processing"); // Macro creates a ScopedBenchmark
        
        std::cout << "Processing a batch of 50,000 orders...\n";
        for (int i = 0; i < 50000; ++i) {
            orders.push_back({i, 150.0 + (i % 100) / 100.0, 10, Order::BUY});
        }
        // Destructor of the benchmark object will be called here
    }
    std::cout << "\nOrder processing finished.\n\n";


    // --- 2. Manual-style benchmark ---
    // Here we have full control over the start and stop times.
    std::cout << "Starting portfolio risk calculation...\n";
    
    Benchmark riskBenchmark("Portfolio Risk Calculation");
    
    riskBenchmark.start();
    
    double calculatedRisk = calculatePortfolioRisk(orders);
    
    riskBenchmark.end();

    // Now we can print the results whenever we want.
    std::cout << riskBenchmark.toString() << std::endl;
    std::cout << "Calculated Portfolio Risk Factor: " << calculatedRisk << "\n\n";

    return 0;
}