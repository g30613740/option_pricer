#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <limits>
#include <random>
#include <stdexcept>

#include "option_pricer/black_scholes.hpp"
#include "option_pricer/monte_carlo.hpp"

namespace option_pricer {
namespace {

TEST(MonteCarlo, AgreesWithBlackScholesWithinSamplingUncertainty) {
  for (auto type : {OptionType::Call, OptionType::Put}) {
    for (double rate : {-0.03, 0.05}) {
      const auto result =
          monte_carlo(100.0, 105.0, rate, 0.25, 1.5, type, 200000, 42);
      EXPECT_GT(result.standard_error, 0.0);
      EXPECT_NEAR(result.price,
                  black_scholes(100.0, 105.0, rate, 0.25, 1.5, type),
                  6.0 * result.standard_error);
    }
  }
}

TEST(MonteCarlo, ComputesDiscountedMeanAndStandardError) {
  // Independent two-pass reference using terminal prices, not discounted GBM.
  for (auto type : {OptionType::Call, OptionType::Put}) {
    for (double rate : {-0.03, 0.05}) {
      std::mt19937_64 generator(123);
      std::normal_distribution<double> normal(0.0, 1.0);
      std::array<double, 8> samples{};
      double mean = 0.0;
      for (double& sample : samples) {
        const double terminal_spot =
            100.0 * std::exp((rate - 0.5 * 0.2 * 0.2) * 1.5 +
                             0.2 * std::sqrt(1.5) * normal(generator));
        sample = std::exp(-rate * 1.5) * payoff(terminal_spot, 100.0, type);
        mean += sample / samples.size();
      }
      double squared_deviations = 0.0;
      for (double sample : samples) {
        squared_deviations += (sample - mean) * (sample - mean);
      }
      const double standard_error =
          std::sqrt(squared_deviations / (samples.size() - 1) / samples.size());
      const auto result =
          monte_carlo(100.0, 100.0, rate, 0.2, 1.5, type, 8, 123);
      EXPECT_NEAR(result.price, mean, 1e-12);
      EXPECT_NEAR(result.standard_error, standard_error, 1e-12);
    }
  }
}

TEST(MonteCarlo, ReproducesResultsWithSameSeed) {
  const auto first =
      monte_carlo(100.0, 100.0, 0.05, 0.2, 1.0, OptionType::Put, 1000, 7);
  const auto second =
      monte_carlo(100.0, 100.0, 0.05, 0.2, 1.0, OptionType::Put, 1000, 7);
  EXPECT_DOUBLE_EQ(first.price, second.price);
  EXPECT_DOUBLE_EQ(first.standard_error, second.standard_error);
  const auto other =
      monte_carlo(100.0, 100.0, 0.05, 0.2, 1.0, OptionType::Put, 1000, 8);
  EXPECT_NE(first.price, other.price);
}

TEST(MonteCarlo, StandardErrorScalesWithInverseSquareRootOfSampleCount) {
  for (auto type : {OptionType::Call, OptionType::Put}) {
    const auto small =
        monte_carlo(100.0, 100.0, 0.05, 0.2, 1.0, type, 4096, 42);
    const auto large =
        monte_carlo(100.0, 100.0, 0.05, 0.2, 1.0, type, 65536, 42);
    EXPECT_NEAR(small.standard_error / large.standard_error, 4.0, 0.5);
  }
}

TEST(MonteCarlo, ReturnsExactBoundaryValuesWithNoSamplingError) {
  for (auto type : {OptionType::Call, OptionType::Put}) {
    for (const auto& parameters :
         {std::array{100.0, 110.0, 0.2, 0.0},
          std::array{100.0, 110.0, 0.0, 1.0},
          std::array{0.0, 110.0, 0.2, 1.0},
          std::array{100.0, 0.0, 0.2, 1.0}}) {
      const auto [spot, strike, volatility, time] = parameters;
      const auto result =
          monte_carlo(spot, strike, -0.05, volatility, time, type, 2, 42);
      EXPECT_DOUBLE_EQ(result.price,
                       black_scholes(spot, strike, -0.05, volatility, time, type));
      EXPECT_DOUBLE_EQ(result.standard_error, 0.0);
    }
  }
}

TEST(MonteCarlo, AcceptsTwoSamples) {
  const auto result =
      monte_carlo(100.0, 100.0, 0.05, 0.2, 1.0, OptionType::Call, 2, 42);
  EXPECT_TRUE(std::isfinite(result.price));
  EXPECT_TRUE(std::isfinite(result.standard_error));
  EXPECT_GE(result.price, 0.0);
  EXPECT_GE(result.standard_error, 0.0);
}

TEST(MonteCarlo, RejectsInvalidInputsEvenAtExpiry) {
  const double infinity = std::numeric_limits<double>::infinity();
  const double nan = std::numeric_limits<double>::quiet_NaN();
  for (auto type : {OptionType::Call, OptionType::Put}) {
    for (double invalid : {-1.0, infinity, -infinity, nan}) {
      EXPECT_THROW(monte_carlo(invalid, 100.0, 0.05, 0.2, 1.0, type, 2, 42),
                   std::invalid_argument);
      EXPECT_THROW(monte_carlo(100.0, invalid, 0.05, 0.2, 1.0, type, 2, 42),
                   std::invalid_argument);
      EXPECT_THROW(monte_carlo(100.0, 100.0, 0.05, invalid, 1.0, type, 2, 42),
                   std::invalid_argument);
      EXPECT_THROW(monte_carlo(100.0, 100.0, 0.05, 0.2, invalid, type, 2, 42),
                   std::invalid_argument);
    }
    for (double invalid : {infinity, -infinity, nan}) {
      EXPECT_THROW(monte_carlo(100.0, 100.0, invalid, 0.2, 0.0, type, 2, 42),
                   std::invalid_argument);
    }
    for (std::size_t count : {0, 1}) {
      EXPECT_THROW(monte_carlo(100.0, 100.0, 0.05, 0.2, 0.0, type, count, 42),
                   std::invalid_argument);
    }
  }
  EXPECT_THROW(monte_carlo(100.0, 100.0, 0.05, 0.2, 1.0,
                           static_cast<OptionType>(42), 2, 42),
               std::invalid_argument);
}

TEST(MonteCarlo, ReportsOverflow) {
  EXPECT_THROW(
      monte_carlo(100.0, 100.0, -1000.0, 0.2, 1.0, OptionType::Put, 2, 42),
      std::overflow_error);
}

}  // namespace
}  // namespace option_pricer
