/*
 * SUMMARY: Implements an Autocall with a safety net ("Airbag").
 * While the early exit logic remains standard, the maturity payoff is modified 
 * to floor potential losses, ensuring the investor recovers at least the 
 * 'airbagFloor' amount even in a worst-case scenario.
 */

#include "AirbagAutocall.hpp"
#include <algorithm>

// Standard setup, just capturing the specific Airbag floor parameter.
AirbagAutocall::AirbagAutocall(std::string underlying,
                               std::vector<double> observationTimes,
                               double spot0, double notional, double couponRate,
                               double callBarrier, double protectionBarrier,
                               double airbagFloor)
    : AutocallBase(std::move(underlying), std::move(observationTimes), spot0,
                   notional, couponRate, callBarrier, protectionBarrier),
      airbagFloor_(airbagFloor) {}

std::vector<CashFlow> AirbagAutocall::cashFlows(const std::vector<double>& path) const {
    std::vector<CashFlow> flows;
    const auto& obs = times();
    const std::size_t steps = std::min(path.size(), obs.size());

    // Check for early redemption (autocall event) at each observation step.
    for (std::size_t i = 0; i < steps; ++i) {
        if (path[i] >= callBarrier()) {
            flows.push_back({notional() * (1.0 + couponRate()), obs[i]});
            return flows; // The product terminates immediately.
        }
    }

    // If we survived until maturity, calculate the final payoff.
    const double finalSpot = (steps > 0) ? path[steps - 1] : spot0();
    flows.push_back({terminalRedemption(finalSpot), obs.back()});
    return flows;
}

double AirbagAutocall::terminalRedemption(double spotT) const {
    // Override base logic to enforce the airbag floor (safety net).
    double base = AutocallBase::terminalRedemption(spotT);
    double minRedemption = notional() * airbagFloor_;
    return std::max(base, minRedemption);
}