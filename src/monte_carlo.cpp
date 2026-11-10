#include "option_pricer/monte_carlo.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <stdexcept>

#include "option_pricer/black_scholes.hpp"

namespace option_pricer {

MonteCarloResult monte_carlo(double spot, double strike, double rate,
                            double volatility, double time_to_expiry,
                            OptionType type, std::size_t num_samples,
                            std::uint64_t seed) {
  if (!std::isfinite(spot) || spot < 0.0 || !std::isfinite(strike) ||
      strike < 0.0 || !std::isfinite(rate) || !std::isfinite(volatility) ||
      volatility < 0.0 || !std::isfinite(time_to_expiry) || time_to_expiry < 0.0 ||
      (type != OptionType::Call && type != OptionType::Put) || num_samples < 2) {
    throw std::invalid_argument("Invalid Monte Carlo inputs");
  }

  if (time_to_expiry == 0.0 || volatility == 0.0 || spot == 0.0 ||
      strike == 0.0) {
    return {black_scholes(spot, strike, rate, volatility, time_to_expiry, type),
            0.0};
  }

  const long double total_volatility =
      static_cast<long double>(volatility) * std::sqrt(time_to_expiry);
  const long double log_discounted_spot =
      std::log(static_cast<long double>(spot)) -
      0.5L * total_volatility * total_volatility;
  const long double discounted_strike =
      std::exp(std::log(static_cast<long double>(strike)) -
               static_cast<long double>(rate) * time_to_expiry);
  const long double max_value = std::numeric_limits<double>::max();
  if (!std::isfinite(log_discounted_spot) ||
      !std::isfinite(discounted_strike) || discounted_strike > max_value) {
    throw std::overflow_error("Monte Carlo parameters exceed supported range");
  }

  std::mt19937_64 generator(seed);
  std::normal_distribution<double> normal(0.0, 1.0);
  long double mean = 0.0L;
  long double sum_squared_deviations = 0.0L;
  for (std::size_t i = 0; i < num_samples; ++i) {
    // GBM: S_T = S * exp((r - sigma^2/2)T + sigma*sqrt(T)*Z).
    // Combine discounting with the exponent to avoid overflow in S_T.
    const long double discounted_spot =
        std::exp(log_discounted_spot + total_volatility * normal(generator));
    if (!std::isfinite(discounted_spot) || discounted_spot > max_value) {
      throw std::overflow_error("Monte Carlo sample exceeds double range");
    }
    const long double sample =
        type == OptionType::Call
            ? std::max(discounted_spot - discounted_strike, 0.0L)
            : std::max(discounted_strike - discounted_spot, 0.0L);

    // Welford's online algorithm: constant memory and stable sample variance.
    const long double delta = sample - mean;
    mean += delta / (static_cast<long double>(i) + 1.0L);
    sum_squared_deviations += delta * (sample - mean);
  }
  const long double standard_error =
      std::sqrt(std::max(sum_squared_deviations, 0.0L) /
                static_cast<long double>(num_samples - 1) /
                static_cast<long double>(num_samples));
  if (!std::isfinite(mean) || !std::isfinite(standard_error) ||
      mean > max_value || standard_error > max_value) {
    throw std::overflow_error("Monte Carlo estimate exceeds double range");
  }
  return {static_cast<double>(mean), static_cast<double>(standard_error)};
}

}  // namespace option_pricer
