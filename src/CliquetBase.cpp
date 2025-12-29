#include "CliquetBase.hpp"
#include <utility>

CliquetBase::CliquetBase(std::string underlying,
                         std::vector<double> observationTimes, double spot0,
                         double notional)
    : StructuredProduct(std::move(underlying), std::move(observationTimes)),
      spot0_(spot0), notional_(notional) {}

double CliquetBase::discountedPayoff(const std::vector<double> &path,
                                     double riskFreeRate) const {
  double amount = payoffImpl(path); // Appelle MaxReturn ou CappedCoupons
  const auto &times = observationTimes();
  double payTime = times.empty() ? 0.0 : times.back();
  return amount * std::exp(-riskFreeRate * payTime);
}