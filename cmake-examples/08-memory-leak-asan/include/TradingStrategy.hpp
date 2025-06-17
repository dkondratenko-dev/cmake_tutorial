// include/TradingStrategy.hpp
#ifndef TRADING_STRATEGY_HPP
#define TRADING_STRATEGY_HPP

#include <string>
#include <memory> // For std::shared_ptr and std::weak_ptr
#include <iostream>

// Forward declaration
class TradingAccount;

class TradingStrategy {
public:
    std::string strategyName;
    std::shared_ptr<TradingAccount> linkedAccount_problematic; // Problematic: will cause cycle
    std::weak_ptr<TradingAccount> linkedAccount_fixed;         // Fixed: breaks cycle

    TradingStrategy(const std::string& name);
    ~TradingStrategy();

    // Problematic method to link account via shared_ptr
    void linkAccountProblematic(std::shared_ptr<TradingAccount> account);
    // Fixed method to link account via weak_ptr
    void linkAccountFixed(std::shared_ptr<TradingAccount> account);

    void display() const;
};

#endif // TRADING_STRATEGY_HPP