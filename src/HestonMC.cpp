/*
 * SUMMARY: Implements the Heston Stochastic Volatility Model.
 * Unlike Black-Scholes (constant vol), this model simulates two coupled processes:
 * 1. The Spot Price (dS).
 * 2. The Variance (dv), which follows a mean-reverting process.
 *
 * It uses a correlation parameter (rho) to link spot and vol shocks (leverage effect)
 * and employs 'sub-stepping' (fine time grid) to ensure numerical stability.
 */

#include "HestonMC.hpp"

#include <algorithm>
#include <cmath>
#include <random>

HestonMC::HestonMC(double v0, double kappa, double theta, double xi, double rho)
    : v0_(v0), kappa_(kappa), theta_(theta), xi_(xi), rho_(rho) {}

std::vector<double> HestonMC::simulatePath(double spot0,
                                           const std::vector<double>& times,
                                           const MarketData& data,
                                           std::mt19937& rng) const {
    std::vector<double> path(times.size());
    const double r = data.riskFreeRate();

    std::normal_distribution<double> dist(0.0, 1.0);

    double spot = spot0;
    double v = v0_; // Initialize the variance process state.
    double prevTime = 0.0;

    // CRITICAL: We use a fixed, small time step (sub-stepping) inside the simulation loop.
    // Why? The observation times (e.g., yearly) are too coarse for the stochastic 
    // variance process, which would become unstable or negative if stepped too largely.
    const double dtStep = 0.01; // Max internal step size (e.g., ~3-4 days).

    for (std::size_t i = 0; i < times.size(); ++i) {
        double currentTime = prevTime;
        const double targetTime = times[i];

        // Advance from the last observation point to the next one using sub-steps.
        while (currentTime < targetTime) {
            // Calculate the actual step size (don't overshoot the target time).
            const double dt = std::min(dtStep, targetTime - currentTime);
            if (dt <= 1e-8) break; // Avoid floating point noise near zero.

            // 1. Generate Correlated Brownian Motions (Cholesky decomposition 2D)
            const double z1 = dist(rng); // Primary noise (dWs) for the Spot.
            const double z2 = dist(rng); // Independent noise.
            
            // Construct the noise for Variance (dWv) using correlation rho.
            // If rho < 0 (typical for equities), spot drops -> vol spikes.
            const double zv = rho_ * z1 + std::sqrt(1.0 - rho_ * rho_) * z2;

            // 2. Update Variance Process (CIR Process)
            // We use the "Full Truncation" scheme (Lord et al.) to handle negative variance.
            // Even though the continuous math says v > 0, the discrete simulation can 
            // push v below 0. We force positive values for the drift/diffusion terms.
            const double v_plus = std::max(v, 0.0);
            const double sqrt_v = std::sqrt(v_plus);

            // dv = Speed(Mean - v)dt + VolOfVol * sqrt(v) * dWv
            v += kappa_ * (theta_ - v_plus) * dt + xi_ * sqrt_v * std::sqrt(dt) * zv;

            // 3. Update Spot Price (Log-Euler discretization)
            // dS = S * r * dt + S * sqrt(v) * dWs
            // Note: We use the geometric solution form for better accuracy.
            spot *= std::exp((r - 0.5 * v_plus) * dt + sqrt_v * std::sqrt(dt) * z1);

            currentTime += dt;
        }

        // Record the spot price at the official observation time.
        path[i] = spot;
        prevTime = targetTime;
    }

    return path;
}