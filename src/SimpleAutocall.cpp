/*
 * SUMMARY: Implements the standard "Vanilla" Autocall.
 * Features a fixed call barrier observed at discrete times. If the barrier is breached, 
 * the product terminates early with a fixed coupon; otherwise, it proceeds to maturity 
 * using the standard protection logic defined in the base class.
 */

#include "SimpleAutocall.hpp"
#include <algorithm>
#include <string>
#include <vector>

SimpleAutocall::SimpleAutocall(std::string underlying,
                               std::vector<double> observationTimes,
                               double spot0,
                               double notional,
                               double couponRate,
                               double callBarrier,
                               double protectionBarrier)
    // Straightforward initialization; simply delegates everything to the base class.
    : AutocallBase(std::move(underlying), std::move(observationTimes), spot0,
                   notional, couponRate, callBarrier, protectionBarrier) {}

std::vector<CashFlow> SimpleAutocall::cashFlows(const std::vector<double>& path) const {
    std::vector<CashFlow> flows;
    const auto& obs = times();
    const std::size_t steps = std::min(path.size(), obs.size());

    // Iterate through observation dates to check for early termination.
    for (std::size_t i = 0; i < steps; ++i) {
        if (path[i] >= callBarrier()) {
            // Trigger condition met: pay capital + yield and stop the product.
            flows.push_back({notional() * (1.0 + couponRate()), obs[i]});
            return flows;
        }
    }

    // No early exit occurred; calculate the final payoff at maturity.
    const double finalSpot = (steps > 0) ? path[steps - 1] : spot0();
    flows.push_back({terminalRedemption(finalSpot), obs.back()});
    return flows;
}