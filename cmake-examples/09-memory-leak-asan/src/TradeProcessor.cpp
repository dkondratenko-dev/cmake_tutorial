// src/TradeProcessor.cpp
#include "../include/TradeProcessor.hpp" // Include its own header
#include <iostream>

TradeProcessor::TradeProcessor() {
    std::cout << "TradeProcessor created." << std::endl;
}

TradeProcessor::~TradeProcessor() {
    std::cout << "TradeProcessor destroyed." << std::endl;
}

Trade* TradeProcessor::createLeakyTrade(const std::string& id, const std::string& symbol, double price, long quantity) {
    Trade* newTrade = new Trade(id, symbol, price, quantity);
    newTrade->setSettlementInstructions("Standard T+2 settlement.");
    std::cout << "  [PROCESSOR] Allocated new Trade " << id << " on heap (raw pointer)." << std::endl;
    return newTrade; // Potential leak point if not deleted by caller
}

std::unique_ptr<Trade> TradeProcessor::createSmartTrade(const std::string& id, const std::string& symbol, double price, long quantity) {
    std::unique_ptr<Trade> smartTrade = std::make_unique<Trade>(id, symbol, price, quantity);
    smartTrade->setSettlementInstructions("Smart pointer managed trade.");
    std::cout << "  [PROCESSOR] Allocated smart Trade " << id << " on heap (unique_ptr)." << std::endl;
    return smartTrade; // Ownership is transferred via unique_ptr
}