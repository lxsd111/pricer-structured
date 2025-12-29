#pragma once
#include "AutocallBase.hpp"

/**
 * @brief Simple Autocall Product Class.
 *
 * Implements the standard "Vanilla" Autocall.
 * - If Spot >= CallBarrier at any observation date: Product stops, pays Notional + Coupon.
 * - If not autocalled by maturity:
 * - If FinalSpot >= ProtectionBarrier: Pay Notional.
 * - Else: Pay Notional * (FinalSpot / Spot0).
 */
class SimpleAutocall : public AutocallBase {
public:
  /**
   * @brief Constructor for SimpleAutocall.
   *
   * @param underlying Name of the underlying asset.
   * @param observationTimes Vector of observation times (in years).
   * @param spot0 Initial spot price.
   * @param notional Notional amount.
   * @param couponRate Fixed coupon rate paid upon autocall.
   * @param callBarrier Autocall barrier level (absolute value).
   * @param protectionBarrier Protection barrier level (absolute value).
   */
  SimpleAutocall(std::string underlying, std::vector<double> observationTimes,
                 double spot0, double notional, double couponRate,
                 double callBarrier, double protectionBarrier);

  /**
   * @brief Calculates the stream of cash flows for a given path.
   *
   * @param path Simulated price path of the underlying.
   * @return std::vector<CashFlow> List of cash flows.
   */
  std::vector<CashFlow> cashFlows(const std::vector<double> &path) const override;
};