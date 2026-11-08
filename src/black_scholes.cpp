#include "option_pricer/black_scholes.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

namespace option_pricer {
namespace {

// Standard normal CDF. erfc avoids subtracting nearly equal values in the tail.
double normal_cdf(double x) {
  return 0.5 * std::erfc(-x / std::numbers::sqrt2);
}

}  // namespace

double black_scholes(double spot, double strike, double rate, double volatility,
                     double time_to_expiry, OptionType type) {
  if (!std::isfinite(spot) || spot < 0.0 || !std::isfinite(strike) ||
      strike < 0.0 || !std::isfinite(rate) || !std::isfinite(volatility) ||
      volatility < 0.0 || !std::isfinite(time_to_expiry) || time_to_expiry < 0.0 ||
      (type != OptionType::Call && type != OptionType::Put)) {
    throw std::invalid_argument("Invalid Black-Scholes inputs");
  }

  if (time_to_expiry == 0.0) {
    return payoff(spot, strike, type);
  }
  if (strike == 0.0) {
    return type == OptionType::Call ? spot : 0.0;
  }

  const double rate_time = rate * time_to_expiry;
  const double discounted_strike = std::exp(std::log(strike) - rate_time);
  if (!std::isfinite(rate_time) || !std::isfinite(discounted_strike)) {
    throw std::overflow_error("Discounted strike exceeds double range");
  }
  if (spot == 0.0) {
    return type == OptionType::Call ? 0.0 : discounted_strike;
  }

  // With no randomness, discounting the terminal payoff gives these limits.
  const double total_volatility = volatility * std::sqrt(time_to_expiry);
  if (!std::isfinite(total_volatility)) {
    throw std::overflow_error("Total volatility exceeds double range");
  }
  if (total_volatility == 0.0) {
    return payoff(spot, discounted_strike, type);
  }

  const double d1 =
      ((std::log(spot) - std::log(strike)) + rate_time) / total_volatility +
      0.5 * total_volatility;
  const double d2 = d1 - total_volatility;

  if (type == OptionType::Call) {
    return std::max(spot * normal_cdf(d1) - discounted_strike * normal_cdf(d2),
                    0.0);
  }
  return std::max(discounted_strike * normal_cdf(-d2) - spot * normal_cdf(-d1),
                  0.0);
}

}  // namespace option_pricer
