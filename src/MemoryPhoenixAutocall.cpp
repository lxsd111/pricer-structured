/*
 * SUMMARY: Implements the "Memory" Phoenix Autocall.
 * Enhances the standard Phoenix logic by caching missed coupons. 
 * If the coupon barrier is breached, the investor recovers the current coupon 
 * PLUS all previously missed coupons ("Memory Effect") in one go.
 */

#include "MemoryPhoenixAutocall.hpp"
#include <algorithm>

MemoryPhoenixAutocall::MemoryPhoenixAutocall(
    std::string underlying, std::vector<double> observationTimes, double spot0,
    double notional, double couponRate, double callBarrier,
    double protectionBarrier, double couponBarrier)
    // Standard initialization; capture the barrier used for the memory trigger.
    : AutocallBase(std::move(underlying), std::move(observationTimes), spot0,
                   notional, couponRate, callBarrier, protectionBarrier),
      couponBarrier_(couponBarrier) {}

std::vector<CashFlow> MemoryPhoenixAutocall::cashFlows(const std::vector<double>& path) const {
    std::vector<CashFlow> flows;
    const auto& obs = times();
    const std::size_t steps = std::min(path.size(), obs.size());

    double accruedCoupons = 0.0;
    const double periodicCoupon = notional() * couponRate();

    for (std::size_t i = 0; i < steps; ++i) {
        // Always add the current period's coupon to the "pending" stack.
        accruedCoupons += periodicCoupon;

        // 1. Check Memory Coupon Trigger
        // If the condition is met, pay out the ENTIRE stack of accrued coupons.
        if (path[i] >= couponBarrier_) {
            flows.push_back({accruedCoupons, obs[i]});
            accruedCoupons = 0.0; // Reset memory after payment.
        }

        // 2. Check Autocall Trigger
        // If we exit early, repay the principal. 
        // Note: The coupon payment (if applicable) was handled in the block above.
        if (path[i] >= callBarrier()) {
            flows.push_back({notional(), obs[i]});
            return flows;
        }
    }

    // Maturity: calculate final redemption (capital protection check).
    const double finalSpot = (steps > 0) ? path[steps - 1] : spot0();
    flows.push_back({terminalRedemption(finalSpot), obs.back()});
    return flows;
}