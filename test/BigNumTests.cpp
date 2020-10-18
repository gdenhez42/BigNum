#include "../src/BigNum.h"
#include <gtest/gtest.h>


TEST(BigNumTests,
     RoundTest)
{

  BigNum b1(std::string("23983094384343.5"));
  BigNum b2(std::string("387409794328709247985743.4"));

  b1.round();
  b2.round();

  EXPECT_EQ(BigNum(std::string("23983094384344")), b1);
  EXPECT_EQ(BigNum(std::string("387409794328709247985743")), b2);
}

TEST(BigNumTests,
     FromStringToString)
{
  std::string nbstr("821974");
  BigNum b1(nbstr);
  std::ostringstream os;
  os << b1;
  EXPECT_EQ(nbstr, os.str());

}

TEST(BigNumTests,
     FromStringToString2)
{
  std::string nbstr("821974293839.250");
  BigNum b1(nbstr);
  std::ostringstream os;
  os << b1;
  EXPECT_EQ(nbstr, os.str());

}

TEST(BigNumTests,
     FromStringToString3)
{
  std::string nbstr("-18.999999999999");
  BigNum b1(nbstr);
  std::ostringstream os;
  os << b1;
  EXPECT_EQ(std::string("-19"), os.str());

}
