#include <benchmark/benchmark.h>

#include "option_pricer/monte_carlo.hpp"

static void BM_MonteCarlo(benchmark::State& state) {
  const auto num_samples = static_cast<std::size_t>(state.range(0));
  const auto type = static_cast<option_pricer::OptionType>(state.range(1));
  for (auto _ : state) {
    benchmark::DoNotOptimize(option_pricer::monte_carlo(
        100.0, 100.0, 0.05, 0.2, 1.0, type, num_samples, 42));
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_MonteCarlo)
    ->Args({1000, 0})
    ->Args({1000, 1})
    ->Args({100000, 0})
    ->Args({100000, 1});
