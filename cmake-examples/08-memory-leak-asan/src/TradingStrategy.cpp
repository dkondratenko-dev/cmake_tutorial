// src/TradingStrategy.cpp
#include "../include/TradingStrategy.hpp"
#include "../include/TradingAccount.hpp" // Needs full definition here

TradingStrategy::TradingStrategy(const std::string& name) : strategyName(name) {
    std::cout << "TradingStrategy " << strategyName << " created." << std::endl;
}

TradingStrategy::~TradingStrategy() {
    std::cout << "TradingStrategy " << strategyName << " destroyed." << std::endl;
}

void TradingStrategy::linkAccountProblematic(std::shared_ptr<TradingAccount> account) {
    linkedAccount_problematic = account;
    std::cout << "  Strategy " << strategyName << " (problematic) linked to account." << std::endl;
}

void TradingStrategy::linkAccountFixed(std::shared_ptr<TradingAccount> account) {
    linkedAccount_fixed = account; // Assigns a weak_ptr from a shared_ptr
    std::cout << "  Strategy " << strategyName << " (fixed) linked to account." << std::endl;
}

void TradingStrategy::display() const {
    std::cout << "  Strategy Name: " << strategyName << std::endl;

    // Corrected check for shared_ptr:
    if (linkedAccount_problematic) { // Directly check if shared_ptr is not null/empty
        std::cout << "    Linked Account (Problematic): " << linkedAccount_problematic->accountId << std::endl;
    } else if (auto account = linkedAccount_fixed.lock()) { // Correct use of lock() for weak_ptr
        std::cout << "    Linked Account (Fixed): " << account->accountId << std::endl;
    } else {
        std::cout << "    No account linked." << std::endl;
    }
}