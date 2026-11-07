#pragma once

namespace option_pricer {

enum class OptionType { Call, Put };

/**
 * @brief Computes a European option payoff at expiry, without discounting.
 * @param spot Underlying price at expiry, finite and nonnegative.
 * @param strike Strike price, finite and nonnegative, in the same units as spot.
 * @param type Call or put.
 * @return Payoff in the same monetary units as spot.
 * @throws std::invalid_argument If a price is negative or nonfinite, or the
 * option type is invalid.
 */
double payoff(double spot, double strike, OptionType type);

}  // namespace option_pricer
