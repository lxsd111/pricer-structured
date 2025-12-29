#pragma once

#include "StructuredProduct.hpp"
#include <string>
#include <vector>

/**
 * @brief Abstract base class for all Autocall products.
 *
 * Stores common parameters like barriers, coupon rates, and observation times.
 * It inherits from StructuredProduct and provides standard getters for the
 * derived classes (Simple, Phoenix, etc.).
 */
class AutocallBase : public StructuredProduct {
public:
    AutocallBase(std::string underlying, std::vector<double> observationTimes,
                 double spot0, double notional, double couponRate,
                 double callBarrier, double protectionBarrier);

    virtual ~AutocallBase() = default;

    // Getters pour les classes dérivées (car les membres sont privés)
    double notional() const { return notional_; }
    double couponRate() const { return couponRate_; }
    double callBarrier() const { return callBarrier_; }
    double protectionBarrier() const { return protectionBarrier_; }
    double spot0() const { return spot0_; }

    // Helper pour accéder aux dates d'observation plus facilement
    const std::vector<double>& times() const { return observationTimes(); }

protected:
    /**
     * @brief Calculates the terminal redemption amount at maturity.
     * Logic: If FinalSpot >= ProtectionBarrier -> Pay Notional
     * Else -> Pay Notional * (FinalSpot / InitialSpot)
     */
    virtual double terminalRedemption(double finalSpot) const;

private:
    double notional_;
    double couponRate_;
    double callBarrier_;
    double protectionBarrier_;
    double spot0_;
};