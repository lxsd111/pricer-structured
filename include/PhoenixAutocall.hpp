#pragma once
#include "AutocallBase.hpp"

/**
 * @brief Phoenix Autocall Product Class.
 *
 * The "Phoenix" feature adds a conditional coupon mechanism.
 * Even if the product is not autocalled, a coupon is paid for a specific period
 * if the spot price is above the 'Coupon Barrier' (which is usually lower than
 * the Autocall Barrier).
 */
class PhoenixAutocall : public AutocallBase {
public:
  /**
   * @brief Constructor for PhoenixAutocall.
   *
   * @param underlying Name of the underlying asset.
   * @param observationTimes Vector of observation times (in years).
   * @param spot0 Initial spot price.
   * @param notional Notional amount.
   * @param couponRate Coupon rate per period.
   * @param callBarrier Autocall barrier level (absolute value).
   * @param protectionBarrier Protection barrier level (absolute value).
   * @param couponBarrier The specific barrier for the conditional coupon payment.
   */
  PhoenixAutocall(std::string underlying, std::vector<double> observationTimes,
                  double spot0, double notional, double couponRate,
                  double callBarrier, double protectionBarrier,
                  double couponBarrier);

  /**
   * @brief Calculates the stream of cash flows for a given path.
   *
   * Logic:
   * 1. Check Autocall: If Spot >= CallBarrier -> Pay Capital + Coupon & Terminate.
   * 2. If not autocalled, check Coupon: If Spot >= CouponBarrier -> Pay Coupon only.
   *
   * @param path Simulated price path of the underlying.
   * @return std::vector<CashFlow> List of cash flows.
   */
  std::vector<CashFlow> cashFlows(const std::vector<double> &path) const override;

private:
  double couponBarrier_;
};