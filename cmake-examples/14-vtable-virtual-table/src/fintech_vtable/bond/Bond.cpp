#include "fintech_vtable/bond/Bond.h"

Bond::Bond(const std::string& id, double faceVal, double coupon)
    : TradableAsset(id), faceValue(faceVal), couponRate(coupon) {}

// The V-Table for Bond will point to this implementation.
// For simplicity, we assume its current value is its face value.
double Bond::getCurrentValue() const {
    return faceValue;
}