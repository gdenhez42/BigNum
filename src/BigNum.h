#ifndef BIGNUM_H
#define BIGNUM_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>


template<size_t T>
class BigNum;

template<size_t T> // declaration
std::ostream& operator<<(std::ostream&, const BigNum<T>&);

// Template param is the number of digit after the comma
template<size_t N>
class BigNum {
  typedef unsigned char Digit;
  typedef std::vector<Digit> Nb;

  Nb m_nb;
  bool m_isNegative;

  size_t size() const { return m_nb.size(); }
  void normalize()
  {
    while(m_nb.size() > 0 && m_nb.back() == 0) m_nb.pop_back();
    if (m_nb.size() == 0) m_isNegative = false;
  }

 public:
  BigNum();
  BigNum(const std::string& p_str);

  // Zero is represented by empty array
  static const BigNum ZERO;
  static BigNum FromString(const std::string& p_str);
  static std::string ToString(const BigNum& p_b, size_t nbDecimals);

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
  friend std::ostream& operator<< <>(std::ostream& os, const BigNum& b);
};

#include "BigNum_impl.h"

#endif

