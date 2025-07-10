#pragma once

#include <string>
#include <iostream>

// Base class for all tradable financial assets.
// This defines the common interface.
class TradableAsset {
public:
    TradableAsset(std::string id) : assetId(std::move(id)) {}

    // A virtual destructor is CRUCIAL for polymorphism.
    virtual ~TradableAsset() {
        std::cout << "Destroying asset: " << assetId << std::endl;
    }

    // The core polymorphic function. Each asset type will calculate its value differently.
    virtual double getCurrentValue() const = 0; // Pure virtual function

    // A non-virtual function, common to all assets.
    const std::string& getId() const {
        return assetId;
    }

protected:
    std::string assetId;
};