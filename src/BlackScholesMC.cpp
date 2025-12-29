/*
 * SUMMARY: Implements the standard Black-Scholes path generator.
 * It assumes constant volatility (Geometric Brownian Motion) to simulate 
 * asset trajectories, serving as the baseline model for pricing.
 */

#include "BlackScholesMC.hpp"
#include "MarketData.hpp"
#include <cmath>
#include <random>
#include <algorithm>

// Constructor just stores the constant volatility.
BlackScholesMC::BlackScholesMC(double sigma) : sigma_(sigma) {}

std::vector<double> BlackScholesMC::simulatePath(double spot0,
                                                 const std::vector<double>& times,
                                                 const MarketData& data,
                                                 std::mt19937& rng) const {
    std::vector<double> path;
    path.reserve(times.size());

    double currentSpot = spot0;
    double currentTime = 0.0;
    const double r = data.riskFreeRate();

    std::normal_distribution<double> dist(0.0, 1.0);

    for (double t : times) {
        double dt = t - currentTime;
        
        // Safety check: prevent negative time steps.
        if (dt < 0.0) dt = 0.0;

        // Only move the spot if time has actually advanced.
        if (dt > 1e-8) {
            const double z = dist(rng);
            const double drift = (r - 0.5 * sigma_ * sigma_) * dt;
            const double diffusion = sigma_ * std::sqrt(dt) * z;
            currentSpot *= std::exp(drift + diffusion);
        }

        path.push_back(currentSpot);
        currentTime = t;
    }

    return path;
}