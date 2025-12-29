/*
 * SUMMARY: Implements a "Capped Cliquet" strategy (Sum of Local Caps).
 * Instead of looking at the global performance (Start to End), this product 
 * sums up the individual performances of each period (e.g., month-to-month).
 * Each period's gain is capped (e.g., max 5% per period), but negative periods 
 * are usually floored at 0 (ratchet effect), protecting past gains.
 */

#include "CliquetCappedCoupons.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

CliquetCappedCoupons::CliquetCappedCoupons(
    std::string underlying,
    std::vector<double> observationTimes,
    double spot0,
    double notional,
    double participation,
    double cap)
    : CliquetBase(std::move(underlying),
                  std::move(observationTimes),
                  spot0,
                  notional),
      participation_(participation),
      cap_(cap) {}

double CliquetCappedCoupons::payoffImpl(
    const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::runtime_error("Cliquet path is empty");
    }
    if (spot0() <= 0.0) {
        return notional(); // Safe fallback for invalid spot.
    }

    double couponSum = 0.0;
    double prevSpot = spot0(); // Initialize with the strike date spot.

    // Iterate through the path to calculate period-by-period returns.
    for (double spot : path) {
        if (prevSpot <= 0.0) {
            prevSpot = spot;
            continue;
        }
        
        // Calculate the raw return for this specific period (e.g., month i vs month i-1).
        const double ret = spot / prevSpot - 1.0;
        
        // Floor negative returns at 0 (local protection).
        const double positiveReturn = std::max(ret, 0.0);
        
        // Apply participation rate and the hard cap for this period.
        const double participated = participation_ * positiveReturn;
        const double coupon = std::clamp(participated, 0.0, cap_);
        
        // Accumulate this period's "locked-in" gain.
        couponSum += coupon;
        
        // Reset the baseline for the next period (Ratchet mechanism).
        prevSpot = spot;
    }

    // Final Payout = Initial Capital + Sum of all locked-in coupons.
    return notional() * (1.0 + couponSum);
}