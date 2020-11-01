#include "../src/BigNum.h"
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

typedef BigNum<1> BigNum1;

RC_GTEST_PROP(AdditionTests,
              AdditionWithIntWorks,
              ())
{
  const int i1 = *rc::gen::inRange(-1073741824, 1073741824);
  const int i2 = *rc::gen::inRange(-1073741824, 1073741824);

  BigNum1 b1(std::to_string(i1));
  BigNum1 b2(std::to_string(i2));
  BigNum1 b3(std::to_string(i1 + i2));

  RC_ASSERT(b3 == b1 + b2);
}

RC_GTEST_PROP(AdditionTests,
              AdditionWithFloatsWorks,
              ())
{
  const double d1 = *rc::gen::inRange(-1000000000, 1000000000);
  const double d2 = *rc::gen::inRange(-1000000000, 1000000000);

  BigNum1 b1(std::to_string(d1));
  BigNum1 b2(std::to_string(d2));
  BigNum1 b3(std::to_string(d1 + d2 - 0.01));
  BigNum1 b4(std::to_string(d1 + d2 + 0.01));
  RC_ASSERT(b3 <= b1 + b2);
  RC_ASSERT(b4 >= b1 + b2);
}

