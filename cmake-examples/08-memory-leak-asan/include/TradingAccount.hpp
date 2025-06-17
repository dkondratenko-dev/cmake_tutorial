// include/TradingAccount.hpp
#ifndef TRADING_ACCOUNT_HPP
#define TRADING_ACCOUNT_HPP

#include <string>
#include <memory> // For std::shared_ptr and std::weak_ptr
#include <iostream>

// Forward declaration to break circular dependency in headers
class TradingStrategy;

class TradingAccount {
public:
    std::string accountId;
    std::shared_ptr<TradingStrategy> currentStrategy; // This will form part of the cycle

    TradingAccount(const std::string& id);
    ~TradingAccount();

    void setStrategy(std::shared_ptr<TradingStrategy> strategy);
    void display() const;
};

#endif // TRADING_ACCOUNT_HPP