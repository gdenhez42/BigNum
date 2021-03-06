#include "../src/BigNum.h"
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

typedef BigNum<1> BigNum1;

TEST(DivisionTests, WhenResultIsANaturalNumber)
{
  BigNum1 b2(std::string("1234567890000000000"));
  BigNum1 b1(std::string("123456789"));

  EXPECT_EQ(BigNum1(std::string("10000000000")), b2/b1);
}

TEST(DivisionTests, WhenResultIsAFloatingPointNumber)
{
  BigNum1 b2(std::string("38474398573298"));
  BigNum1 b1(std::string("3498342394"));

  EXPECT_EQ(BigNum1("10997.891"), b2/b1);
}

TEST(DivisionTests, WhenDividingFloatingPointNumbers)
{
  BigNum1 b2(std::string("932874943398439573.2324"));
  BigNum1 b1(std::string("9384303259.232"));

  EXPECT_EQ(BigNum1("99408013.3206165"), b2/b1);
}

RC_GTEST_PROP(DivisionTests, DivisionByItselfGivesOne, ())
{
  const double d = *rc::gen::inRange(-1000000000, 1000000000);

  BigNum1 b(std::to_string(d));
  RC_ASSERT(BigNum1("1") == b/b);
}

RC_GTEST_PROP(DivisionTests, ZeroDividedBySomethingGivesZero, ())
{
  const double d = *rc::gen::inRange(1, 1000000000);

  BigNum1 b(std::to_string(d));
  RC_ASSERT(BigNum1() == BigNum1()/b);
}

RC_GTEST_PROP(DivisionTests, DivisionWithIntsWorks, ())
{
  const int i1 = *rc::gen::inRange(-10000, 10000);
  const int i2 = *rc::gen::inRange(1, 10000);

  BigNum1 b2(std::to_string(i1*i2));
  BigNum1 b1(std::to_string(i2));

  EXPECT_EQ(BigNum1(std::to_string(i1)), b2/b1);
}

RC_GTEST_PROP(DivisionTests,
              DivisionWithFloatsWorks,
              ())
{
  const double d1 = *rc::gen::inRange(-1000000000, 1000000000);
  const double d2 = *rc::gen::inRange(1, 1000000000);

  BigNum1 b1(std::to_string(d1));
  BigNum1 b2(std::to_string(d2));
  BigNum1 b3(std::to_string(d1/d2 - 0.01));
  BigNum1 b4(std::to_string(d1/d2 + 0.01));
  RC_ASSERT(b3 <= b1/b2);
  RC_ASSERT(b4 >= b1/b2);
}

