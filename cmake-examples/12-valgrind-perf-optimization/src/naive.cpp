// src/naive.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <numeric>
#include <chrono>

// Parameters for the option and simulation
struct OptionData {
    double initial_price; // S0
    double strike_price;  // K
    double risk_free_rate; // r
    double volatility;    // sigma
    double time_to_maturity; // T in years
    int num_simulations;
    int num_steps;
};

// BAD: This function creates and seeds a new generator on every call.
// This is a major performance killer in a loop.
double generate_normal_random() {
    // Static to ensure it's only seeded once per program run,
    // but a new distribution is created on each call, which is still inefficient.
    // A better naive example would put this whole block inside the function without static.
    // For a clearer bad example, let's pretend we are creating the engine every time.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0.0, 1.0);
    return d(gen);
}

double run_monte_carlo(const OptionData& data) {
    double total_payoff = 0.0;
    double dt = data.time_to_maturity / data.num_steps;
    double drift = (data.risk_free_rate - 0.5 * data.volatility * data.volatility) * dt;
    double diffusion = data.volatility * std::sqrt(dt);

    for (int i = 0; i < data.num_simulations; ++i) {
        // VERY BAD: Unnecessary heap allocation in a tight loop.
        // We only need the final price, not the whole path.
        std::vector<double> path;
        path.push_back(data.initial_price);

        for (int j = 0; j < data.num_steps; ++j) {
            double epsilon = generate_normal_random(); // Inefficient call
            double next_price = path.back() * std::exp(drift + diffusion * epsilon);
            path.push_back(next_price);
        }

        // Calculate payoff for this single simulation
        double final_price = path.back();
        double payoff = std::max(final_price - data.strike_price, 0.0);
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
    // data.num_simulations = 10000; // Keep it low for a quick example run
    data.num_simulations = 1000; // Keep it low for a quick example run
    data.num_steps = 252; // Number of trading days in a year

    auto start = std::chrono::high_resolution_clock::now();
    double price = run_monte_carlo(data);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Naive Implementation" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "Calculated Option Price: " << price << std::endl;
    std::cout << "Execution Time: " << duration.count() << " ms" << std::endl;

    return 0;
}