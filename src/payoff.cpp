#include "option_pricer/payoff.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace option_pricer {

double payoff(double spot, double strike, OptionType type) {
  if (!std::isfinite(spot) || !std::isfinite(strike) || spot < 0.0 ||
      strike < 0.0) {
    throw std::invalid_argument("Spot and strike must be finite and nonnegative");
  }

  switch (type) {
    case OptionType::Call:
      return std::max(spot - strike, 0.0);
    case OptionType::Put:
      return std::max(strike - spot, 0.0);
  }

  throw std::invalid_argument("Unknown option type");
}

}  // namespace option_pricer
