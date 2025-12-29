#pragma once
#include "StructuredProduct.hpp"
#include <string>
#include <vector>

/**
 * @brief Base class for Cliquet-style products.
 *
 * Implements the "Template Method" pattern:
 * - cashFlows() handles the timing (payment at maturity).
 * - payoffImpl() (virtual) handles the specific math (MaxReturn, Capped, etc.).
 */
class CliquetBase : public StructuredProduct {
public:
  CliquetBase(std::string underlying, std::vector<double> observationTimes,
              double spot0, double notional);

  virtual ~CliquetBase() = default;

  // Adaptation : On renvoie un vecteur contenant 1 seul flux (le payoff final)
  std::vector<CashFlow> cashFlows(const std::vector<double> &path) const override;

  // Getters (utiles pour les classes dérivées comme MaxReturn ou CappedCoupons)
  double spot0() const { return spot0_; }
  double notional() const { return notional_; }

protected:
  // Helper pour accéder aux dates
  const std::vector<double> &times() const { return observationTimes(); }

  /**
   * @brief Pure virtual function to calculate the payoff amount.
   * Must be implemented by derived classes (e.g., MaxReturn, CappedCoupons).
   */
  virtual double payoffImpl(const std::vector<double> &path) const = 0;

private:
  double spot0_{};
  double notional_{};
};