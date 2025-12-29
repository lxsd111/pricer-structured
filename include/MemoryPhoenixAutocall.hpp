#pragma once
#include "AutocallBase.hpp"

/**
 * @brief Memory Phoenix Autocall Product Class.
 *
 * Similar to Phoenix, but recovers missed coupons ("Memory effect") 
 * once the coupon barrier is crossed.
 */
class MemoryPhoenixAutocall : public AutocallBase {
public:
  MemoryPhoenixAutocall(std::string underlying,
                        std::vector<double> observationTimes, double spot0,
                        double notional, double couponRate, double callBarrier,
                        double protectionBarrier, double couponBarrier);

  /**
   * @brief Calculates the stream of cash flows.
   * Logic includes the "Memory" effect: accumulating unpaid coupons.
   */
  std::vector<CashFlow> cashFlows(const std::vector<double> &path) const override;

private:
  double couponBarrier_{};
};