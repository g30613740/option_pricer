#pragma once

#include <cstddef>
#include <cstdint>

#include "option_pricer/payoff.hpp"

namespace option_pricer {

/** @brief Monte Carlo price and sampling uncertainty, in monetary units. */
struct MonteCarloResult {
  double price;           ///< Mean discounted payoff.
  double standard_error;  ///< Estimated standard deviation of the price estimate.
};

/**
 * @brief Prices a European option without dividends by Monte Carlo under GBM.
 *
 * Samples independent terminal prices under the risk-neutral Black-Scholes
 * model, with constant rate and volatility. Returns exact limits with zero
 * standard error at expiry or when volatility, spot or strike is zero.
 * Standard error measures sampling uncertainty, not model error; it is not a
 * guaranteed error bound. The same seed reproduces results on the same standard
 * library implementation; normal distributions may differ across platforms.
 *
 * @param spot Current underlying price, finite and nonnegative.
 * @param strike Strike price, finite and nonnegative, in the same units as spot.
 * @param rate Finite annual continuously compounded rate as a decimal.
 * @param volatility Finite nonnegative annual volatility as a decimal.
 * @param time_to_expiry Finite nonnegative time remaining in years.
 * @param type Call or put.
 * @param num_samples Number of independent scenarios, at least two.
 * @param seed Seed for the local random number generator.
 * @return Estimated present value and standard error in the same units as spot.
 * @throws std::invalid_argument If any input violates the above conditions.
 * @throws std::overflow_error If a calculation exceeds the supported range.
 */
MonteCarloResult monte_carlo(double spot, double strike, double rate,
                            double volatility, double time_to_expiry,
                            OptionType type, std::size_t num_samples,
                            std::uint64_t seed);

}  // namespace option_pricer
