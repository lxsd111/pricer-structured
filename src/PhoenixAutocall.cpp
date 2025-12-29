// src/PhoenixAutocall.cpp
#include "PhoenixAutocall.hpp"
#include <algorithm>

PhoenixAutocall::PhoenixAutocall(std::string underlying,
                                 std::vector<double> observationTimes,
                                 double spot0, double notional,
                                 double couponRate, double callBarrier,
                                 double protectionBarrier, double couponBarrier)
    : AutocallBase(std::move(underlying), std::move(observationTimes), spot0,
                   notional, couponRate, callBarrier, protectionBarrier),
      couponBarrier_(couponBarrier) {}

double PhoenixAutocall::discountedPayoff(const std::vector<double> &path,
                                         double riskFreeRate) const {
  double totalValue = 0.0;
  const auto &obs = times();
  const std::size_t steps = std::min(path.size(), obs.size());

<<<<<<< HEAD
    for (std::size_t i = 0; i < steps; ++i) {
        // 1. Check Autocall Condition
        if (path[i] >= callBarrier()) {
            // Early redemption: Notional + Coupon
            flows.push_back({notional() * (1.0 + couponRate()), obs[i]});
            return flows;
        }

        // 2. Check Coupon Condition (Phoenix specific)
        // Even if not autocalled, we get a coupon if above couponBarrier
        if (path[i] >= couponBarrier_) {
            flows.push_back({notional() * couponRate(), obs[i]});
        }
=======
  for (std::size_t i = 0; i < steps; ++i) {
    // Coupon
    if (path[i] >= couponBarrier_) {
      totalValue +=
          (notional() * couponRate()) * std::exp(-riskFreeRate * obs[i]);
>>>>>>> refactor/payoff-calculation
    }
    // Autocall
    if (path[i] >= callBarrier()) {
      totalValue += notional() * std::exp(-riskFreeRate * obs[i]);
      return totalValue;
    }
  }

<<<<<<< HEAD
    // Maturity
    const double finalSpot = (steps > 0) ? path[steps - 1] : spot0();
    flows.push_back({terminalRedemption(finalSpot), obs.back()});
    return flows;
=======
  // Maturité
  const double finalSpot = (steps > 0) ? path[steps - 1] : spot0();
  totalValue +=
      terminalRedemption(finalSpot) * std::exp(-riskFreeRate * obs.back());
  return totalValue;
>>>>>>> refactor/payoff-calculation
}