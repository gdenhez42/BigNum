#include "../src/BigNum.h"
#include <gtest/gtest.h>

typedef BigNum<1> BigNum1;
typedef BigNum<3> BigNum3;

TEST(BigNumTests,
     RoundTest1)
{

  BigNum1 b1(std::string("23983094384343.5"));
  BigNum1 b2(std::string("387409794328709247985743.4"));

  b1.round();
  b2.round();

  EXPECT_EQ(BigNum1(std::string("23983094384344")), b1);
  EXPECT_EQ(BigNum1(std::string("387409794328709247985743")), b2);
}
TEST(BigNumTests,
     RoundTest3)
{

  BigNum3 b1(std::string("0.928"));

  b1.round();

  EXPECT_EQ(BigNum3(std::string("1")), b1);
}
TEST(BigNumTests,
     FromStringToString)
{
  std::string nbstr("821974");
  BigNum1 b1(nbstr);
  EXPECT_EQ(nbstr, BigNum1::ToString(b1, 0));

}

TEST(BigNumTests,
     FromStringToString2)
{
  std::string nbstr("821974293839.250");
  BigNum1 b1(nbstr);
  EXPECT_EQ(nbstr, BigNum1::ToString(b1, 3));
}

TEST(BigNumTests,
     FromStringToString3)
{
  std::string nbstr("-18.999999999999");
  BigNum1 b1(nbstr);
  EXPECT_EQ(std::string("-18.99"), BigNum1::ToString(b1,2));

}
