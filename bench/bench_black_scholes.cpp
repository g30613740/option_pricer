#include <benchmark/benchmark.h>

#include "option_pricer/black_scholes.hpp"

static void BM_BlackScholes(benchmark::State& state) {
  double spot = 100.0;
  const auto type = static_cast<option_pricer::OptionType>(state.range(0));
  for (auto _ : state) {
    benchmark::DoNotOptimize(spot);
    benchmark::DoNotOptimize(
        option_pricer::black_scholes(spot, 100.0, 0.05, 0.2, 1.0, type));
  }
}
BENCHMARK(BM_BlackScholes)->Arg(0)->Arg(1);
