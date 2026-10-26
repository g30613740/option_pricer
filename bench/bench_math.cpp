#include <benchmark/benchmark.h>

#include "option_pricer/math.hpp"

static void BM_Add(benchmark::State& state) {
  double a = 1.0;
  double b = 2.0;
  for (auto _ : state) {
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(option_pricer::add(a, b));
  }
}
BENCHMARK(BM_Add);
