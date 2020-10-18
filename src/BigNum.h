#ifndef BIGNUM_H
#define BIGNUM_H

#include <vector>
#include <string>
#include <iostream>

typedef unsigned char Digit;
typedef std::vector<Digit> BigNat;

class BigNum {
  // First elem in m_nb is a floating point digit
  // Zero is represented by an array of one element with value 0
  BigNat m_nb;
  bool m_isNegative;

  size_t size() const { return m_nb.size(); }

 public:
  BigNum();
  BigNum(const std::string& p_str);

  static const BigNum ZERO;

  void round();

  BigNum& operator+=(const BigNum&);
  BigNum& operator-=(const BigNum&);
  BigNum& operator*=(const BigNum&);
  BigNum& operator/=(const BigNum&);

  bool operator<(const BigNum&) const;
  bool operator>(const BigNum& o) const { return o < *this; }
  bool operator<=(const BigNum& o) const { return !(*this > o); }
  bool operator>=(const BigNum& o) const { return !(*this < o); }
  bool operator==(const BigNum& o) const;

  friend BigNum operator+(BigNum b1, const BigNum& b2)
  {
    b1 += b2;
    return b1;
  }
  friend BigNum operator-(BigNum b1, const BigNum& b2)
  {
    b1 -= b2;
    return b1;
  }
  friend BigNum operator*(BigNum b1, const BigNum& b2)
  {
    b1 *= b2;
    return b1;
  }
  friend BigNum operator/(BigNum b1, const BigNum& b2)
  {
    b1 /= b2;
    return b1;
  }
  friend std::ostream& operator<<(std::ostream& os, const BigNum& b);
};

#endif

