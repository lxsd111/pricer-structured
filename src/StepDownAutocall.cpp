/*
 * SUMMARY: Implements the Step-Down Autocall mechanism.
 * The key difference is the dynamic barrier levels: instead of a fixed target, 
 * the threshold for early redemption lowers at each observation date (via a vector),
 * mechanically increasing the probability of an early exit as time passes.
 */

#include "StepDownAutocall.hpp"
#include <algorithm>
#include <utility>
#include <vector>

StepDownAutocall::StepDownAutocall(std::string underlying,
                                   std::vector<double> observationTimes,
                                   double spot0, double notional,
                                   double couponRate,
                                   std::vector<double> callBarriers,
                                   double protectionBarrier)
    // Pass the first barrier to the base class for safety, but we'll rely on the vector.
    : AutocallBase(std::move(underlying), std::move(observationTimes), spot0,
                   notional, couponRate,
                   callBarriers.empty() ? 0.0 : callBarriers.front(),
                   protectionBarrier),
      callBarriers_(std::move(callBarriers)) {}

std::vector<CashFlow> StepDownAutocall::cashFlows(const std::vector<double>& path) const {
    std::vector<CashFlow> flows;
    const auto &obs = times();
    const std::size_t steps = std::min(path.size(), obs.size());

    for (std::size_t i = 0; i < steps; ++i) {
        // Retrieve the barrier level for this period. 
        // If the vector is shorter than the path, stick to the last defined barrier.
        double currentBarrier = callBarriers_.empty() ? callBarrier() :
            callBarriers_[std::min(i, callBarriers_.size() - 1)];

        // Check against the current (likely lower) barrier level.
        if (path[i] >= currentBarrier) {
            flows.push_back({notional() * (1.0 + couponRate()), obs[i]});
            return flows;
        }
    }

    // No autocall occurred; handle maturity.
    const double finalSpot = (steps > 0) ? path[steps - 1] : spot0();
    flows.push_back({terminalRedemption(finalSpot), obs.back()});
    return flows;
}