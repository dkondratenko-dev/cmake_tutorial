#pragma once

#include "fintech_vtable/TradableAsset.h"

class Bond : public TradableAsset {
public:
    Bond(const std::string& id, double faceValue, double coupon);

    // Override the pure virtual function from the base class.
    double getCurrentValue() const override;

private:
    double faceValue;
    double couponRate;
};