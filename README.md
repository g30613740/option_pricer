# Option Pricer

[![CI](https://github.com/g30613740/option_pricer/actions/workflows/ci.yml/badge.svg)](https://github.com/g30613740/option_pricer/actions/workflows/ci.yml)

A C++ library for pricing financial options. Implements call/put payoffs,
Black–Scholes (analytic) and Monte Carlo pricing for European options without
dividends. A binomial tree is planned. The command-line application compares
analytic and simulated call/put prices at two sample counts.

The Black–Scholes API accepts a continuously compounded annual rate and annual
volatility as decimals (`0.05` means 5%), and time to expiry in years. It supports
zero time, zero volatility, zero spot/strike and negative rates. Invalid inputs
throw `std::invalid_argument`; intermediate overflow throws `std::overflow_error`.

Monte Carlo uses the same risk-neutral model as Black–Scholes. It samples
independent terminal prices, averages discounted payoffs and returns
`MonteCarloResult {price, standard_error}`. It needs at least two scenarios and
an explicit random seed. Reusing the seed reproduces the estimate with the same
standard library implementation; results may differ across platforms.

The standard error estimates sampling uncertainty in price units, not model
accuracy or a guaranteed error bound. It generally decreases as `1/sqrt(N)`:
four times as many scenarios roughly halves the error and costs four times as
much work. For sufficiently large samples, `price +/- 1.96 * standard_error`
is an approximate 95% confidence interval under the model. Rarely sampled
payoffs can make this approximation unreliable. Expiry, zero volatility, zero
spot and zero strike use exact limits with zero standard error.

A step-by-step explanation with numerical examples and a map to the C++ code
is available in [the Monte Carlo walkthrough](docs/monte_carlo_walkthrough.md)
(in Russian).

**Documentation:** https://g30613740.github.io/option_pricer/

## Project structure

| Path | Contents |
|---|---|
| `include/option_pricer/` | Public headers of the library |
| `src/` | Library sources |
| `app/` | Command-line application |
| `tests/` | Unit tests (GoogleTest) |
| `bench/` | Benchmarks (Google Benchmark) |
| `docs/` | Doxygen configuration |
| `.github/workflows/` | CI: build, tests, benchmark, documentation |

## Requirements

- CMake ≥ 3.20
- C++20 compiler (GCC ≥ 11 or Clang ≥ 14)
- Doxygen (only for documentation)

GoogleTest and Google Benchmark are downloaded automatically by CMake.

## Build and run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/option_pricer_app
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

## Benchmark

```bash
./build/bench/benchmarks
```

## Documentation

```bash
doxygen docs/Doxyfile   # output: build/docs/html/index.html
```

On every push to `main`, CI builds the documentation and publishes it to GitHub Pages.
