# AGENTS.md

Instructions for coding agents working in this repository.

## Purpose

`option-pricer` is a C++20 library for pricing financial options (Black–Scholes,
Monte Carlo, binomial tree) with a command-line application, unit tests,
benchmarks and Doxygen documentation.

## Structure

- `include/option_pricer/` — public headers (`.hpp`), namespace `option_pricer`
- `src/` — implementation (`.cpp`)
- `app/` — command-line application
- `tests/` — GoogleTest unit tests, one file per module: `test_<module>.cpp`
- `bench/` — Google Benchmark benchmarks: `bench_<module>.cpp`
- `docs/Doxyfile` — documentation config
- `.github/workflows/ci.yml` — CI pipeline

## Commands

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release   # configure
cmake --build build                              # build
ctest --test-dir build --output-on-failure       # unit tests
./build/bench/benchmarks                         # benchmark
doxygen docs/Doxyfile                            # documentation
```

## Code style

- C++20, Google style, formatting by `.clang-format` (`clang-format -i <file>`).
- Files: `snake_case`; types: `PascalCase`; functions and variables: `snake_case`.
- Every public function in `include/` has a Doxygen comment (`@brief`, `@param`, `@return`).
- No compiler warnings with `-Wall -Wextra -Wpedantic`.
- New functionality comes with unit tests; performance-critical code also gets a benchmark.
- Financial formulas: state the model and units in the comment (rates and volatility
  are annual, as decimals: `0.05`, not `5`).

## Rules

- Before finishing a task: build the project and run all tests; they must pass.
- Do not change CI/CD (`.github/workflows/`) or `docs/Doxyfile` without an explicit request.
- Do not add new external dependencies without an explicit request.
- Do not commit build artifacts (`build/`) or secrets.
- Do not push to the remote repository; the owner pushes after review.
- Keep changes minimal and focused on the task; do not refactor unrelated code.
