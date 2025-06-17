// src/TradingAccount.cpp
#include "../include/TradingAccount.hpp"
#include "../include/TradingStrategy.hpp" // Needs full definition here

TradingAccount::TradingAccount(const std::string& id) : accountId(id) {
    std::cout << "TradingAccount " << accountId << " created." << std::endl;
}

TradingAccount::~TradingAccount() {
    std::cout << "TradingAccount " << accountId << " destroyed." << std::endl;
}

void TradingAccount::setStrategy(std::shared_ptr<TradingStrategy> strategy) {
    currentStrategy = strategy;
    std::cout << "  Account " << accountId << " linked to strategy." << std::endl;
}

void TradingAccount::display() const {
    std::cout << "  Account ID: " << accountId << std::endl;
    if (currentStrategy) {
        std::cout << "    Linked Strategy: " << currentStrategy->strategyName << std::endl;
    } else {
        std::cout << "    No strategy linked." << std::endl;
    }
}