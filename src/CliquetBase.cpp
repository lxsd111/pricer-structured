/*
 * SUMMARY: Base class for Cliquet-style products.
 * Unlike Autocalls which can exit early, Cliquets typically run to maturity.
 * This class handles the common infrastructure and the single payment timing,
 * relying on the "Template Method" pattern (payoffImpl) for the specific payoff logic.
 */

#include "CliquetBase.hpp"
#include <utility>

CliquetBase::CliquetBase(std::string underlying,
                         std::vector<double> observationTimes,
                         double spot0,
                         double notional)
    : underlying_(std::move(underlying)),
      observationTimes_(std::move(observationTimes)),
      spot0_(spot0),
      notional_(notional) {}

std::vector<CashFlow> CliquetBase::cashFlows(const std::vector<double>& path) const {
    // Delegate the specific path-dependent math (e.g., Sum of Caps, Max Return)
    // to the derived class implementation.
    double amount = payoffImpl(path); 
    
    // Cliquets usually have a single cash flow at the very end (maturity).
    double payTime = observationTimes_.empty() ? 0.0 : observationTimes_.back();
    return {{amount, payTime}};
}

const std::vector<double>& CliquetBase::observationTimes() const {
    return observationTimes_;
}

const std::string& CliquetBase::underlying() const {
    return underlying_;
}