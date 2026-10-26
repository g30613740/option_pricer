# Option Pricer

[![CI](https://github.com/g30613740/option_pricer/actions/workflows/ci.yml/badge.svg)](https://github.com/g30613740/option_pricer/actions/workflows/ci.yml)

A C++ library for pricing financial options. Planned models: Black–Scholes (analytic),
Monte Carlo and binomial tree. The current version contains a placeholder function
that is used to set up the build, tests, benchmarks and documentation.

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
