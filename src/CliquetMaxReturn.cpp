/*
 * SUMMARY: Implements a "Lookback" or "Best-Of" strategy.
 * Instead of relying on the spot price at maturity, this product tracks 
 * the performance at every observation date relative to the initial spot.
 * The final payout is based on the highest return ("High Water Mark") 
 * achieved at any point during the product's lifetime.
 */

#include "CliquetMaxReturn.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

CliquetMaxReturn::CliquetMaxReturn(std::string underlying,
                                   std::vector<double> observationTimes,
                                   double spot0,
                                   double notional)
    : CliquetBase(std::move(underlying),
                  std::move(observationTimes),
                  spot0,
                  notional) {}

double CliquetMaxReturn::payoffImpl(const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::runtime_error("Cliquet path is empty");
    }
    if (spot0() <= 0.0) {
        return 0.0;
    }

    double maxReturn = 0.0;

    // Iterate through the entire history of the path.
    for (double spot : path) {
        // Calculate the global return relative to the initial strike (t=0).
        // Unlike the "CappedCoupons" (which was local/ratchet), this is global.
        const double ratio = spot / spot0() - 1.0;
        
        // Update the "High Water Mark" if the current spot is the best seen so far.
        maxReturn = std::max(maxReturn, ratio);
    }

    // The payout matches the single best performance observed.
    // Note: This specific implementation pays the gain only (not Capital + Gain).
    return notional() * std::max(maxReturn, 0.0);
}