// include/Trade.hpp
#ifndef TRADE_HPP
#define TRADE_HPP

#include <string>
#include <iostream> // For std::cout (though often moved to .cpp for large projects)

// Represents a single financial trade
struct Trade {
    std::string tradeId;
    std::string symbol;
    double price;
    long quantity;
    char* settlementInstructions; // Dynamically allocated string

    Trade(const std::string& id, const std::string& sym, double p, long q);
    ~Trade(); // Destructor to free dynamically allocated memory within the Trade object

    void setSettlementInstructions(const std::string& instructions);
    void display() const;
};

#endif // TRADE_HPP