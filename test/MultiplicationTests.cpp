#include "../src/BigNum.h"
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

typedef BigNum<1> BigNum1;

TEST(MultiplicationTests, SomethingTimesZeroEqualsZero)
{
  BigNum1 b1(std::string("-123456789"));
  BigNum1 b2(std::string("0"));

  EXPECT_EQ(b1*b2, b2);
  EXPECT_EQ(b2*b1, b2);
}


RC_GTEST_PROP(MultiplicationTests,
              MultiplicationWithIntWorks,
              ())
{
  const int i1 = *rc::gen::inRange(-46340, 46340);
  const int i2 = *rc::gen::inRange(-46340, 46340);

  BigNum1 b1(std::to_string(i1));
  BigNum1 b2(std::to_string(i2));
  BigNum1 b3(std::to_string(i1 * i2));

  RC_ASSERT(b3 == b1 * b2);
}
