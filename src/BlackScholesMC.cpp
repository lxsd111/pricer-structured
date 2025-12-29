#include "BlackScholesMC.hpp"
#include "MarketData.hpp"
#include <cmath>
#include <random>

BlackScholesMC::BlackScholesMC(double sigma) : sigma_(sigma) {}

std::vector<double>
BlackScholesMC::simulatePath(double spot0, const std::vector<double> &times,
                             const MarketData &data, std::mt19937 &rng) const {
  std::vector<double> path;
  path.reserve(times.size());

  double currentSpot = spot0;
  double currentTime = 0.0;
  double r = data.riskFreeRate();

  std::normal_distribution<double> d(0.0, 1.0);

  for (double t : times) {
    double dt = t - currentTime;
    if (dt < 0.0)
      dt = 0.0;

    if (dt > 1e-8) {
      double z = d(rng);
      double drift = (r - 0.5 * sigma_ * sigma_) * dt;
      double diffusion = sigma_ * std::sqrt(dt) * z;
      currentSpot *= std::exp(drift + diffusion);
    }

    path.push_back(currentSpot);
    currentTime = t;
  }

  return path;
}