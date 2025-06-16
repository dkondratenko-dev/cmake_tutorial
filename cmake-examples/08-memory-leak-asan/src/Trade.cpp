// src/Trade.cpp
#include "../include/Trade.hpp" // Include its own header
#include <cstring> // For strcpy

Trade::Trade(const std::string& id, const std::string& sym, double p, long q)
    : tradeId(id), symbol(sym), price(p), quantity(q), settlementInstructions(nullptr) {
    std::cout << "Trade " << tradeId << " created." << std::endl;
}

Trade::~Trade() {
    if (settlementInstructions) {
        delete[] settlementInstructions;
        settlementInstructions = nullptr;
        std::cout << "Settlement instructions for " << tradeId << " freed." << std::endl;
    }
    std::cout << "Trade " << tradeId << " destroyed." << std::endl;
}

void Trade::setSettlementInstructions(const std::string& instructions) {
    if (settlementInstructions) {
        delete[] settlementInstructions; // Free existing memory BEFORE allocating new
        settlementInstructions = nullptr;
    }
    settlementInstructions = new char[instructions.length() + 1];
    strcpy(settlementInstructions, instructions.c_str());
}

void Trade::display() const {
    std::cout << "  ID: " << tradeId << ", Symbol: " << symbol
              << ", Price: " << price << ", Quantity: " << quantity;
    if (settlementInstructions) {
        std::cout << ", Instructions: " << settlementInstructions;
    }
    std::cout << std::endl;
}