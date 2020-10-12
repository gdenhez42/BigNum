#include "BigNum.h"
#include <algorithm>
#include <iostream>

namespace {
  int Compare(const BigNat& nb1, const BigNat& nb2)
  {
    if (nb1.size() != nb2.size()) {
      return nb1.size() < nb2.size() ? -1 : 1;
    }
    for (size_t i = 1; i <= nb1.size(); i++) {
      if (nb1[nb1.size() - i] != nb2[nb2.size() - i]) {
        return nb1[nb1.size() - i] < nb2[nb2.size() - i] ? -1 : 1;
      }
    }
    return 0;
  }

  void Add(BigNat& nb,
           Digit toAdd)
  {
    int temp = toAdd;
    for (size_t i = 0; i < nb.size() && temp > 0; i++) {
      temp += nb[i];
      nb[i] = temp % 256;
      temp /= 256;
    }
    if (temp > 0) nb.push_back(temp);
  }

  void Add(BigNat& nb1,
           const BigNat& nb2)
  {
    Digit retenue = 0;
    size_t max = std::max(nb1.size(), nb2.size());
    for (size_t i = 0; i < max; i++) {
      int temp = retenue;
      if (i < nb1.size()) temp += nb1[i];
      if (i < nb2.size()) temp += nb2[i];
      retenue = temp / 256;
      temp %= 256;

      if (i < nb1.size()) nb1[i] = temp;
      else nb1.push_back(temp);
    }

    if (retenue != 0) nb1.push_back(retenue);
  }

  // Assume nb1 larger than m
  void Minus(BigNat& nb1,
             const BigNat& nb2) {
    for (size_t i = 0; i < nb2.size(); i++) {
      if (nb1[i] < nb2[i]) {
        nb1[i] = 256 - nb2[i] + nb1[i];
        size_t j = i + 1;
        while (nb1[j] == 0) {
          nb1[j] = 255;
          j++;
        }
        nb1[j] -= 1;
      } else {
        nb1[i] -= nb2[i];
      }
    }
    while(nb1.back() == 0 && nb1.size() > 1) nb1.pop_back();
  }

  BigNat Mult(const BigNat& nb,
              Digit num)
  {
    BigNat result;
    int temp = 0;
    for (size_t i = 0; i < nb.size(); i++) {
      temp = nb[i] * num + temp;
      result.push_back(temp % 256);
      temp /= 256;
    }
    if (temp > 0) {
      result.push_back(temp);
    }

    return result;
  }

  // Assume result between 1 and 255
  Digit Div(const BigNat& nb1,
            const BigNat& nb2)
  {
    Digit step = 128;
    Digit test = step;
    Digit result = 1;
    while (step > 0) {
      BigNat temp = Mult(nb2, test);
      int compRes = Compare(temp, nb1);
      if (compRes == 0) return test;
      step /= 2;
      if (compRes == -1) {
        result = test;
        test += step;
      } else {
        test -= step;
      }
    }
    return result;
  }
}

const BigNum BigNum::ZERO = BigNum();

BigNum::BigNum()
  : m_nb(1),
    m_frac(0),
    m_isNegative(false)
{
}


BigNum::BigNum(const std::string& p_str)
  : m_nb(),
    m_frac(0),
    m_isNegative(false)
{

  if (p_str.size() == 0) return;

  size_t index = 0;
  if (p_str[0] == '-') {
    m_isNegative = true;
    index ++;
  }

  std::vector<unsigned char> base10;
  while (index < p_str.size()) {
    if (p_str[index] == '.') {
      index++;
      break;
    }

    base10.push_back(p_str[index] - 48);
    index++;
  }

  while (base10.size() > 0) {
    int mod = 0;
    std::vector<unsigned char> result;
    for (size_t i = 0; i < base10.size(); i++) {
      mod *= 10;
      mod += base10[i];
      if (result.size() > 0 || mod >= 256) result.push_back(mod / 256);
      mod = mod % 256;
    }
    m_nb.push_back(mod);
    base10 = std::move(result);
  }

  base10.clear();
  while (index < p_str.size()) {
    base10.push_back(p_str[index] - 48);
    index++;
  }

  double temp = 0;
  double factor = 1;
  for (size_t i = 0; i < base10.size(); i++) {
    factor *= 10;
    temp += base10[i] * 256 / factor;
  }
  if (temp + 0.5 > 256) {
    Add(m_nb, 1);
  }

  m_frac = static_cast<unsigned char>(temp + 0.5);
}

BigNum& BigNum::operator+=(const BigNum& other)
{
  if (m_isNegative != other.m_isNegative) {
    BigNum temp = other;
    temp.m_isNegative = !other.m_isNegative;
    *this -= temp;
    return *this;
  }
  int temp = m_frac + other.m_frac;
  m_frac = temp % 256;
  temp /= 256;

  Add(m_nb, temp);
  Add(m_nb, other.m_nb);

  return *this;
}

BigNum& BigNum::operator-=(const BigNum& other)
{
  if (m_isNegative != other.m_isNegative) {
    BigNum temp = other;
    temp.m_isNegative = !other.m_isNegative;
    *this += temp;
    return *this;
  }

  BigNum temp;
  if (*this < other ^ m_isNegative) {
    temp = *this;
    *this = other;
    m_isNegative = !m_isNegative;
  } else {
    temp = other;
  }

  if (m_frac < temp.m_frac) {
    m_frac = m_frac + 256 - temp.m_frac;
    size_t index = 0;
    while (m_nb[index] == 0) {
      m_nb[index] = 255;
      index++;
    }
    m_nb[index] -= 1;
  } else {
    m_frac -= temp.m_frac;
  }

  Minus(m_nb, temp.m_nb);

  return *this;
}

BigNum& BigNum::operator*=(const BigNum& other) {

  if (*this == ZERO) {
    return *this;
  }

  if (other == ZERO) {
    m_nb = BigNat(1,0);
    m_frac = 0;
    m_isNegative = false;
    return *this;
  }

  m_isNegative = m_isNegative ^ other.m_isNegative;

  BigNat result;

  int temp = m_frac * other.m_frac;
  result.push_back(temp % 256);
  temp /= 256;
  for (size_t i = 0; i < size(); i++) {
    temp += (m_nb[i] * other.m_frac);
    result.push_back(temp % 256);
    temp /= 256;
  }
  if (temp > 0) result.push_back(temp);

  for (size_t j = 0; j < other.size(); j++) {

    BigNat rTemp(j+1, 0);
    temp = m_frac * other.m_nb[j];
    rTemp.push_back(temp % 256);
    temp /= 256;
    for (size_t i = 0; i < size(); i++) {
      temp += (m_nb[i] * other.m_nb[j]);
      rTemp.push_back(temp % 256);
      temp /= 256;
    }
    if (temp > 0) rTemp.push_back(temp);
    Add(result, rTemp);
  }

  m_frac = result[1];
  m_nb = BigNat(result.begin() + 2, result.end());

  return *this;
}

BigNum& BigNum::operator/=(const BigNum& other)
{
  if (other == ZERO) {
    throw "Division by zero";
  }

  m_isNegative = m_isNegative ^ other.m_isNegative;

  BigNat rest(1, m_frac);
  if (m_nb != BigNat(1,0)) {
    rest.insert(rest.end(), m_nb.begin(), m_nb.end());
  }

  BigNat div(1, other.m_frac);
  if (other.m_nb != BigNat(1,0)) {
    div.insert(div.end(), other.m_nb.begin(), other.m_nb.end());
  }

  BigNat temp(1, rest.back());
  rest.pop_back();
  while (Compare(temp, div) < 0 && rest.size() != 0) {
    temp.insert(temp.begin(), rest.back());
    rest.pop_back();
  }

  if (Compare(temp, div) < 0) {
    m_nb = BigNat(1,0);
  } else {
    m_nb = BigNat(rest.size() + 1);
    size_t index = m_nb.size();

    Digit digit = Div(temp, div);
    Minus(temp, Mult(div, digit));
    m_nb[index-1] = digit;
    index--;

    while (rest.size() > 0) {
      if (temp == BigNat(1,0)) {
        temp[0] = rest.back();
      } else {
        temp.insert(temp.begin(), rest.back());
      }
      rest.pop_back();

      if (Compare(temp, div) >= 0) {
        Digit digit = Div(temp, div);
        Minus(temp, Mult(div, digit));
        m_nb[index-1] = digit;
      }

      index--;
    }
  }

  if (temp != BigNat(1,0)) {
    temp.insert(temp.begin(), 0);
    m_frac = Div(temp, div);
  } else {
    m_frac = 0;
  }

  return *this;
}

bool BigNum::operator<(const BigNum& other) const
{
  if (m_isNegative != other.m_isNegative) {
    return m_isNegative;
  }

  int result = Compare(m_nb, other.m_nb);
  if (result == -1) return !m_isNegative;
  if (result == 1) return m_isNegative;

  if (m_frac < other.m_frac) return !m_isNegative;
  if (m_frac > other.m_frac) return m_isNegative;

  return false;
}

bool BigNum::operator==(const BigNum& o) const
{
  return m_isNegative == o.m_isNegative &&
    m_nb == o.m_nb &&
    m_frac == o.m_frac;
}

std::ostream& operator<<(std::ostream& os, const BigNum& b)
{
  std::vector<unsigned char> base256 = b.m_nb;

  for (size_t i = 0; i < base256.size() / 2; i++) {
    std::swap(base256[i], base256[base256.size() - 1 - i]);
  }

  std::vector<unsigned char> base10;

  while (base256.size() > 0) {
    int mod = 0;
    std::vector<unsigned char> result;
    for (size_t i = 0; i < base256.size(); i++) {
      mod *= 256;
      mod += base256[i];
      if (result.size() > 0 || mod >= 10) result.push_back(mod / 10);
      mod = mod % 10;
    }
    base10.push_back(mod);
    base256 = std::move(result);
  }

  if (b.m_isNegative) {
    os << '-';
  }
  for (size_t i = 1; i <= base10.size(); i++) {
    os << (int)base10[base10.size() - i];
  }

  if (b.m_frac != 0) {
    char buffer[4];
    int fracBase10 = static_cast<int>(b.m_frac / 256.0 * 1000 + 0.5);
    sprintf(buffer, "%03d", fracBase10);
    os << '.' << buffer;
  }
  return os;
}

