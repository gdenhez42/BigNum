#include "../src/BigNum.h"
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

TEST(DivisionTests, WhenResultIsANaturalNumber)
{
  BigNum b2(std::string("1234567890000000000"));
  BigNum b1(std::string("123456789"));
  BigNum actual = b2/b1;
  BigNum expected(std::string("10000000000"));

  EXPECT_EQ(expected, actual);
}

RC_GTEST_PROP(DivisionTests, WorksWhenResultIs1Digits, ())
{
  const int i1 = *rc::gen::inRange(1, 255);
  const int i2 = *rc::gen::inRange(1, 255);

  BigNum b2(std::to_string(i1*i2));
  BigNum b1(std::to_string(i1));

  EXPECT_EQ(BigNum(std::to_string(i2)), b2/b1);
}

