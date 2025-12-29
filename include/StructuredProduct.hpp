#pragma once

#include <string>
#include <vector>

class StructuredProduct {
public:
  StructuredProduct(std::string underlying,
                    std::vector<double> observationTimes)
      : underlying_(std::move(underlying)),
        observationTimes_(std::move(observationTimes)) {}

  virtual ~StructuredProduct() = default;

  // Calcule directement le payoff total actualisé pour un chemin donné
  virtual double discountedPayoff(const std::vector<double> &path,
                                  double riskFreeRate) const = 0;

  const std::vector<double> &observationTimes() const {
    return observationTimes_;
  }
  const std::string &underlying() const { return underlying_; }

private:
  std::string underlying_;
  std::vector<double> observationTimes_;
};