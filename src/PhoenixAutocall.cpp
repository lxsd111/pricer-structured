/*
 * SUMMARY: Implements the "Phoenix" Autocall logic.
 * Unlike the standard Autocall (which usually accumulates coupons until exit), 
 * the Phoenix pays coupons periodically as long as the underlying stays above 
 * a specific 'couponBarrier', even if the product continues running.
 */

#include "PhoenixAutocall.hpp"
#include <algorithm>

PhoenixAutocall::PhoenixAutocall(std::string underlying,
                                 std::vector<double> observationTimes,
                                 double spot0,
                                 double notional,
                                 double couponRate,
                                 double callBarrier,
                                 double protectionBarrier,
                                 double couponBarrier)
    // Initialize base, then store the specific barrier for conditional coupon payments.
    : AutocallBase(std::move(underlying), std::move(observationTimes), spot0,
                   notional, couponRate, callBarrier, protectionBarrier),
      couponBarrier_(couponBarrier) {}

std::vector<CashFlow> PhoenixAutocall::cashFlows(const std::vector<double>& path) const {
    std::vector<CashFlow> flows;
    const auto& obs = times();
    const std::size_t steps = std::min(path.size(), obs.size());

    for (std::size_t i = 0; i < steps; ++i) {
        // 1. Check Autocall Condition (Priority #1)
        if (path[i] >= callBarrier()) {
            // Success: Pay capital + current coupon and terminate immediately.
            flows.push_back({notional() * (1.0 + couponRate()), obs[i]});
            return flows;
        }

        // 2. Check Coupon Condition (Phoenix specific)
        // If we are here, we didn't autocall. However, we still check if 
        // the spot is high enough to warrant a coupon payment for this period.
        if (path[i] >= couponBarrier_) {
            flows.push_back({notional() * couponRate(), obs[i]});
        }
    }

    // No early exit occurred; calculate the final redemption at maturity.
    const double finalSpot = (steps > 0) ? path[steps - 1] : spot0();
    flows.push_back({terminalRedemption(finalSpot), obs.back()});
    return flows;
}