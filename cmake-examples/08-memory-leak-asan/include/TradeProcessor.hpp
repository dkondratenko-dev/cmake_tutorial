// include/TradeProcessor.hpp
#ifndef TRADEPROCESSOR_HPP
#define TRADEPROCESSOR_HPP

#include <string>
#include <memory> // For std::unique_ptr
#include "Trade.hpp" // TradeProcessor interacts with Trade objects

// A class that might expose memory leaks if its functions return raw pointers
class TradeProcessor {
public:
    TradeProcessor();
    ~TradeProcessor();

    // Problematic function: creates a Trade object dynamically and returns a raw pointer.
    // The CALLER is responsible for deleting this returned pointer.
    Trade* createLeakyTrade(const std::string& id, const std::string& symbol, double price, long quantity);

    // Recommended: Function using smart pointers for automatic memory management.
    // This demonstrates good practice and prevents leaks.
    std::unique_ptr<Trade> createSmartTrade(const std::string& id, const std::string& symbol, double price, long quantity);
};

#endif // TRADEPROCESSOR_HPP