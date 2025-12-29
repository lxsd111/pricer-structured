/*
 * SUMMARY: Defines the standard Black-Scholes path generator.
 * It assumes constant volatility (Geometric Brownian Motion) to simulate 
 * asset trajectories, serving as the baseline model for pricing.
 */

#pragma once

#include "PathModel.hpp"

class BlackScholesMC : public PathModelBase {
public:
    // Constructor captures the constant volatility parameter used for the entire path.
    explicit BlackScholesMC(double sigma);

    // Generates a single discrete path (Geometric Brownian Motion) 
    // across the specified observation times using the stored sigma.
    std::vector<double> simulatePath(double spot0,
                                     const std::vector<double>& times,
                                     const MarketData& data,
                                     std::mt19937& rng) const override;
private:
    double sigma_;
};