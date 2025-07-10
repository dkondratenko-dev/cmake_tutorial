#include "fintech_vtable/stock/Stock.h"

Stock::Stock(const std::string& id, std::string symbol, int shares, double price)
    : TradableAsset(id), 
      tickerSymbol(std::move(symbol)), 
      numShares(shares), 
      pricePerShare(price) {}

// The V-Table for Stock will point to this implementation.
double Stock::getCurrentValue() const {
    return numShares * pricePerShare;
}