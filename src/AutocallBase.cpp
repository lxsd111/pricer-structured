/*
 * SUMMARY: The base class for all Autocall variations.
 * It stores shared characteristics (barriers, notional, initial spot) and
 * implements the standard "Capital at Risk" logic at maturity:
 * 100% capital back if above protection, otherwise linear loss.
 */

#include "AutocallBase.hpp"

AutocallBase::AutocallBase(std::string underlying,
                           std::vector<double> observationTimes,
                           double spot0,
                           double notional,
                           double couponRate,
                           double callBarrier,
                           double protectionBarrier)
    : StructuredProduct(std::move(underlying), std::move(observationTimes)),
      notional_(notional), couponRate_(couponRate),
      callBarrier_(callBarrier), protectionBarrier_(protectionBarrier),
      spot0_(spot0) {}

double AutocallBase::terminalRedemption(double finalSpot) const {
    // Check if the protection barrier (Knock-In) was breached at maturity.
    if (finalSpot >= protectionBarrier_) {
        return notional_; // Barrier intact: Capital is safe.
    }
    
    // Barrier breached: Capital is at risk.
    // The redemption mirrors the underlying's performance (e.g., -40% spot -> -40% capital).
    return notional_ * (finalSpot / spot0_);
}