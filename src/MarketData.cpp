/*
 * SUMMARY: A simple container for the market snapshot at time t=0.
 * It centralizes the risk-free rate and per-asset data (spot prices, volatilities)
 * to ensure all pricing models reference the same consistent baseline.
 */

#include "MarketData.hpp"
#include <stdexcept>

void MarketData::setRiskFreeRate(double r) {
    riskFreeRate_ = r; // Global constant rate used for discounting.
}

double MarketData::riskFreeRate() const {
    return riskFreeRate_;
}

void MarketData::setQuote(const std::string& underlying, const Quote& quote) {
    // Stores or updates the spot/vol for a specific asset (e.g., "SX5E").
    quotes_[underlying] = quote;
}

const MarketData::Quote& MarketData::getQuote(const std::string& underlying) const {
    auto it = quotes_.find(underlying);
    if (it == quotes_.end()) {
        // Fail hard if the pricer requests an asset we don't have data for.
        throw std::runtime_error("MarketData: Underlying not found: " + underlying);
    }
    return it->second;
}