#pragma once

#include "option_pricer/payoff.hpp"

namespace option_pricer {

/**
 * @brief Prices a European option in the Black-Scholes model without dividends.
 *
 * Assumes constant volatility and a constant continuously compounded rate.
 * At expiry returns payoff; at zero volatility uses the deterministic limit.
 *
 * @param spot Current underlying price, finite and nonnegative.
 * @param strike Strike price, finite and nonnegative, in the same units as spot.
 * @param rate Annual continuously compounded rate as a decimal; may be negative.
 * @param volatility Annual volatility as a decimal, finite and nonnegative.
 * @param time_to_expiry Time remaining in years, finite and nonnegative.
 * @param type Call or put.
 * @return Present option value in the same monetary units as spot.
 * @throws std::invalid_argument If inputs violate the above conditions.
 * @throws std::overflow_error If intermediate calculations exceed double range.
 */
double black_scholes(double spot, double strike, double rate, double volatility,
                     double time_to_expiry, OptionType type);

}  // namespace option_pricer
