#include <iomanip>
#include <iostream>

#include "option_pricer/black_scholes.hpp"

int main() {
  using option_pricer::OptionType;
  using option_pricer::black_scholes;

  std::cout << "Black-Scholes (European options, no dividends)\n"
            << "S=100, K=100, r=5%, volatility=20%, T=1 year\n"
            << std::fixed << std::setprecision(6)
            << "Call: " << black_scholes(100.0, 100.0, 0.05, 0.2, 1.0,
                                        OptionType::Call)
            << '\n'
            << "Put:  " << black_scholes(100.0, 100.0, 0.05, 0.2, 1.0,
                                        OptionType::Put)
            << '\n';
  return 0;
}
