#include <benchmark/benchmark.h>

#include "option_pricer/payoff.hpp"

static void BM_Payoff(benchmark::State& state) {
  double spot = 120.0;
  double strike = 100.0;
  const auto type = static_cast<option_pricer::OptionType>(state.range(0));
  for (auto _ : state) {
    benchmark::DoNotOptimize(spot);
    benchmark::DoNotOptimize(strike);
    benchmark::DoNotOptimize(option_pricer::payoff(spot, strike, type));
  }
}
BENCHMARK(BM_Payoff)->Arg(0)->Arg(1);
