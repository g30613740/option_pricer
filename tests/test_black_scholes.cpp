#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <stdexcept>

#include "option_pricer/black_scholes.hpp"

namespace option_pricer {
namespace {

TEST(BlackScholes, MatchesReferencePrices) {
  EXPECT_NEAR(black_scholes(100.0, 100.0, 0.05, 0.2, 1.0, OptionType::Call),
              10.450583572185565, 1e-10);
  EXPECT_NEAR(black_scholes(100.0, 100.0, 0.05, 0.2, 1.0, OptionType::Put),
              5.573526022256971, 1e-10);
  // Published example: https://www.mathworks.com/help/finance/blsprice.html
  EXPECT_NEAR(black_scholes(100.0, 95.0, 0.1, 0.5, 0.25, OptionType::Call),
              13.6953, 5e-5);
  EXPECT_NEAR(black_scholes(100.0, 95.0, 0.1, 0.5, 0.25, OptionType::Put),
              6.3497, 5e-5);
}

TEST(BlackScholes, SatisfiesPutCallParityAndPriceBounds) {
  for (double spot : {50.0, 100.0, 150.0}) {
    for (double rate : {-0.05, 0.0, 0.05}) {
      for (double volatility : {0.01, 0.2, 1.0}) {
        const double call =
            black_scholes(spot, 100.0, rate, volatility, 2.0, OptionType::Call);
        const double put =
            black_scholes(spot, 100.0, rate, volatility, 2.0, OptionType::Put);
        const double discounted_strike = 100.0 * std::exp(-rate * 2.0);
        EXPECT_NEAR(call - put, spot - discounted_strike, 1e-10);
        EXPECT_GE(call, 0.0);
        EXPECT_LE(call, spot);
        EXPECT_GE(put, 0.0);
        EXPECT_LE(put, discounted_strike + 1e-10);
        EXPECT_GE(call + 1e-10, spot - discounted_strike);
        EXPECT_GE(put + 1e-10, discounted_strike - spot);
      }
    }
  }
}

TEST(BlackScholes, EqualsPayoffAtExpiry) {
  for (auto type : {OptionType::Call, OptionType::Put}) {
    for (double spot : {0.0, 80.0, 100.0, 120.0}) {
      EXPECT_DOUBLE_EQ(black_scholes(spot, 100.0, 0.05, 0.2, 0.0, type),
                       payoff(spot, 100.0, type));
    }
  }
}

TEST(BlackScholes, HandlesZeroVolatility) {
  for (double rate : {-0.05, 0.0, 0.05}) {
    for (auto type : {OptionType::Call, OptionType::Put}) {
      for (double spot : {80.0, 100.0, 120.0}) {
        EXPECT_NEAR(black_scholes(spot, 100.0, rate, 0.0, 1.0, type),
                    payoff(spot, 100.0 * std::exp(-rate), type), 1e-10);
      }
    }
  }
}

TEST(BlackScholes, HandlesZeroSpotAndStrike) {
  EXPECT_DOUBLE_EQ(black_scholes(0.0, 100.0, 0.05, 0.2, 1.0, OptionType::Call),
                   0.0);
  EXPECT_NEAR(black_scholes(0.0, 100.0, 0.05, 0.2, 1.0, OptionType::Put),
              100.0 * std::exp(-0.05), 1e-10);
  EXPECT_DOUBLE_EQ(black_scholes(100.0, 0.0, 0.05, 0.2, 1.0, OptionType::Call),
                   100.0);
  EXPECT_DOUBLE_EQ(black_scholes(100.0, 0.0, 0.05, 0.2, 1.0, OptionType::Put),
                   0.0);
  for (auto type : {OptionType::Call, OptionType::Put}) {
    EXPECT_DOUBLE_EQ(black_scholes(0.0, 0.0, 0.05, 0.2, 1.0, type), 0.0);
  }
}

TEST(BlackScholes, RejectsInvalidInputs) {
  const double infinity = std::numeric_limits<double>::infinity();
  const double nan = std::numeric_limits<double>::quiet_NaN();
  for (auto type : {OptionType::Call, OptionType::Put}) {
    for (double invalid : {-1.0, infinity, -infinity, nan}) {
      EXPECT_THROW(black_scholes(invalid, 100.0, 0.05, 0.2, 1.0, type),
                   std::invalid_argument);
      EXPECT_THROW(black_scholes(100.0, invalid, 0.05, 0.2, 1.0, type),
                   std::invalid_argument);
      EXPECT_THROW(black_scholes(100.0, 100.0, 0.05, invalid, 1.0, type),
                   std::invalid_argument);
      EXPECT_THROW(black_scholes(100.0, 100.0, 0.05, 0.2, invalid, type),
                   std::invalid_argument);
    }
    for (double invalid : {infinity, -infinity, nan}) {
      EXPECT_THROW(black_scholes(100.0, 100.0, invalid, 0.2, 1.0, type),
                   std::invalid_argument);
    }
  }
  EXPECT_THROW(black_scholes(100.0, 100.0, 0.05, 0.2, 1.0,
                             static_cast<OptionType>(42)),
               std::invalid_argument);
  // Validation applies even when the expiry branch could return immediately.
  EXPECT_THROW(black_scholes(100.0, 100.0, nan, 0.2, 0.0, OptionType::Call),
               std::invalid_argument);
}

TEST(BlackScholes, ReportsOverflow) {
  EXPECT_THROW(black_scholes(100.0, 100.0, -1000.0, 0.2, 1.0, OptionType::Put),
               std::overflow_error);
}

}  // namespace
}  // namespace option_pricer
