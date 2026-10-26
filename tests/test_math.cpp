#include <gtest/gtest.h>

#include "option_pricer/math.hpp"

TEST(Add, SumsPositiveNumbers) { EXPECT_DOUBLE_EQ(option_pricer::add(2.0, 3.0), 5.0); }

TEST(Add, HandlesNegativeNumbers) { EXPECT_DOUBLE_EQ(option_pricer::add(-1.5, 0.5), -1.0); }
