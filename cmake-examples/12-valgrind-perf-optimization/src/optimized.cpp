// src/optimized.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <numeric>
#include <chrono>

struct OptionData {
    double initial_price;
    double strike_price;
    double risk_free_rate;
    double volatility;
    double time_to_maturity;
    int num_simulations;
    int num_steps;
};

// GOOD: The core simulation now takes the generator by reference.
// No new objects are created inside.
double run_monte_carlo(const OptionData& data, std::mt19937& gen, std::normal_distribution<>& dist) {
    double total_payoff = 0.0;
    double dt = data.time_to_maturity / data.num_steps;
    double drift = (data.risk_free_rate - 0.5 * data.volatility * data.volatility) * dt;
    double diffusion = data.volatility * std::sqrt(dt);

    for (int i = 0; i < data.num_simulations; ++i) {
        // GOOD: No more vector. Just track the current price.
        double current_price = data.initial_price;

        for (int j = 0; j < data.num_steps; ++j) {
            double epsilon = dist(gen); // EFFICIENT: just use the existing generator
            current_price *= std::exp(drift + diffusion * epsilon);
        }

        double payoff = std::max(current_price - data.strike_price, 0.0);
        total_payoff += payoff;
    }

    double mean_payoff = total_payoff / data.num_simulations;
    double discounted_price = mean_payoff * std::exp(-data.risk_free_rate * data.time_to_maturity);
    return discounted_price;
}

int main() {
    OptionData data;
    data.initial_price = 100.0;
    data.strike_price = 105.0;
    data.risk_free_rate = 0.05;
    data.volatility = 0.20;
    data.time_to_maturity = 1.0;
    // data.num_simulations = 10000;
    data.num_simulations = 1000;
    data.num_steps = 252;

    // GOOD: Create the random number generator and distribution ONCE.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0, 1.0);

    auto start = std::chrono::high_resolution_clock::now();
    double price = run_monte_carlo(data, gen, dist); // Pass them in
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Optimized Implementation" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "Calculated Option Price: " << price << std::endl;
    std::cout << "Execution Time: " << duration.count() << " ms" << std::endl;

    return 0;
}