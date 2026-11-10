#include <iomanip>
#include <iostream>

#include "option_pricer/black_scholes.hpp"
#include "option_pricer/monte_carlo.hpp"

int main() {
  using option_pricer::OptionType;
  using option_pricer::black_scholes;

  std::cout << "European options, no dividends\n"
            << "S=100, K=100, r=5%, volatility=20%, T=1 year\n"
            << "Monte Carlo: independent scenarios, seed=42\n"
            << std::fixed << std::setprecision(6);
  for (auto type : {OptionType::Call, OptionType::Put}) {
    std::cout << (type == OptionType::Call ? "Call" : "Put")
              << " Black-Scholes: "
              << black_scholes(100.0, 100.0, 0.05, 0.2, 1.0, type) << '\n';
    for (std::size_t num_samples : {10000, 100000}) {
      const auto result = option_pricer::monte_carlo(
          100.0, 100.0, 0.05, 0.2, 1.0, type, num_samples, 42);
      std::cout << "  Monte Carlo (" << num_samples << " scenarios): "
                << result.price << ", standard error: " << result.standard_error
                << '\n';
    }
  }
  return 0;
}
