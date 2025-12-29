#pragma once
#include "AutocallBase.hpp"
#include <vector>

/**
 * @brief Step-Down Autocall Product Class.
 *
 * Features a dynamic autocall barrier that decreases over time (e.g., 100%, 95%, 90%...).
 * This increases the probability of early redemption as the product nears maturity.
 */
class StepDownAutocall : public AutocallBase {
public:
  /**
   * @brief Constructor for StepDownAutocall.
   *
   * @param underlying Name of the underlying asset.
   * @param observationTimes Vector of observation times.
   * @param spot0 Initial spot price.
   * @param notional Notional amount.
   * @param couponRate Coupon rate paid upon autocall.
   * @param callBarriers Vector of barrier levels corresponding to each observation time.
   * @param protectionBarrier Protection barrier level (constant).
   */
  StepDownAutocall(std::string underlying, std::vector<double> observationTimes,
                   double spot0, double notional, double couponRate,
                   std::vector<double> callBarriers, double protectionBarrier);

  /**
   * @brief Calculates the stream of cash flows for a given path.
   *
   * Logic:
   * - At step i, check if Path[i] >= CallBarriers[i].
   * - If yes -> Pay Notional + Coupon & Terminate.
   *
   * @param path Simulated price path.
   * @return std::vector<CashFlow> List of cash flows.
   */
  std::vector<CashFlow> cashFlows(const std::vector<double> &path) const override;

private:
  std::vector<double> callBarriers_;
};