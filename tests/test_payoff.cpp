#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

#include "option_pricer/payoff.hpp"

namespace option_pricer {
namespace {

TEST(Payoff, CallBelowAtAndAboveStrike) {
  EXPECT_DOUBLE_EQ(payoff(80.0, 100.0, OptionType::Call), 0.0);
  EXPECT_DOUBLE_EQ(payoff(100.0, 100.0, OptionType::Call), 0.0);
  EXPECT_DOUBLE_EQ(payoff(120.0, 100.0, OptionType::Call), 20.0);
}

TEST(Payoff, PutBelowAtAndAboveStrike) {
  EXPECT_DOUBLE_EQ(payoff(80.0, 100.0, OptionType::Put), 20.0);
  EXPECT_DOUBLE_EQ(payoff(100.0, 100.0, OptionType::Put), 0.0);
  EXPECT_DOUBLE_EQ(payoff(120.0, 100.0, OptionType::Put), 0.0);
}

TEST(Payoff, AcceptsZeroPrices) {
  for (const auto type : {OptionType::Call, OptionType::Put}) {
    EXPECT_DOUBLE_EQ(payoff(0.0, 0.0, type), 0.0);
  }
  EXPECT_DOUBLE_EQ(payoff(0.0, 100.0, OptionType::Call), 0.0);
  EXPECT_DOUBLE_EQ(payoff(0.0, 100.0, OptionType::Put), 100.0);
  EXPECT_DOUBLE_EQ(payoff(100.0, 0.0, OptionType::Call), 100.0);
  EXPECT_DOUBLE_EQ(payoff(100.0, 0.0, OptionType::Put), 0.0);
}

TEST(Payoff, RejectsNegativeAndNonfinitePrices) {
  const double infinity = std::numeric_limits<double>::infinity();
  const double nan = std::numeric_limits<double>::quiet_NaN();
  for (const auto type : {OptionType::Call, OptionType::Put}) {
    for (const double invalid_price : {-1.0, infinity, -infinity, nan}) {
      EXPECT_THROW(payoff(invalid_price, 100.0, type), std::invalid_argument);
      EXPECT_THROW(payoff(100.0, invalid_price, type), std::invalid_argument);
    }
  }
}

TEST(Payoff, RejectsInvalidOptionType) {
  EXPECT_THROW(payoff(100.0, 100.0, static_cast<OptionType>(42)),
               std::invalid_argument);
}

}  // namespace
}  // namespace option_pricer
