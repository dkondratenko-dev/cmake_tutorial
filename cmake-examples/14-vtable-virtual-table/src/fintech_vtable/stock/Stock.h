#pragma once

#include "fintech_vtable/TradableAsset.h"

class Stock : public TradableAsset {
public:
    Stock(const std::string& id, std::string symbol, int shares, double price);

    // Override the pure virtual function from the base class.
    double getCurrentValue() const override;

private:
    std::string tickerSymbol;
    int numShares;
    double pricePerShare;
};