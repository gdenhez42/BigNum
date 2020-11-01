
template<size_t N>
const BigNum<N> BigNum<N>::ZERO = BigNum();

template<size_t N>
BigNum<N>::BigNum()
  : m_nb(),
    m_isNegative(false)
{
}

template<size_t N>
BigNum<N>::BigNum(const std::string& p_str)
  : m_nb(),
    m_isNegative(false)
{
  *this = FromString(p_str);
}

template<size_t N>
BigNum<N> BigNum<N>::FromString(const std::string& p_str)
{
  // todo: Validation

  BigNum<N> b;
  b.m_nb.insert(b.m_nb.begin(), N, 0);

  size_t index = 0;
  if (p_str[0] == '-') {
    b.m_isNegative = true;
    index ++;
  }

  Nb base10;
  while (index < p_str.size()) {
    if (p_str[index] == '.') {
      index++;
      break;
    }

    base10.push_back(p_str[index] - 48);
    index++;
  }

  // Convert integer part to base 256
  while (base10.size() > 0) {
    int mod = 0;
    Nb result;
    for (size_t i = 0; i < base10.size(); i++) {
      mod *= 10;
      mod += base10[i];
      if (result.size() > 0 || mod >= 256) result.push_back(mod / 256);
      mod = mod % 256;
    }
    b.m_nb.push_back(mod);
    base10 = std::move(result);
  }

  base10.clear();
  while (index < p_str.size()) {
    base10.push_back(p_str[index] - 48);
    index++;
  }

  // Convert fraction part to base 256
  for (size_t i = 0; i < N; i++) {
    int retenue = 0;
    Nb result;
    for (Nb::const_reverse_iterator it = base10.rbegin();
         it != base10.rend();
         ++it) {
      retenue += *it * 256;
      result.push_back(retenue % 10);
      retenue /= 10;
    }
    b.m_nb[N - 1 - i] = retenue;
    base10 = std::move(result);
  }

  b.normalize();

  return b;
}

template<size_t N>
std::string BigNum<N>::ToString(const BigNum<N>& p_b, size_t nbDecimals)
{
  std::string result;
  if (p_b.m_isNegative) {
    result.push_back('-');
  }
  // Convert integer part to base 10
  if (p_b.size() <= N) {
    result.push_back('0');
  } else {
    Nb base256(p_b.m_nb.rbegin(), p_b.m_nb.rbegin() + (p_b.size() - N));

    Nb rTemp;
    while (base256.size() > 0) {
      int mod = 0;
      Nb tRes;
      for (size_t i = 0; i < base256.size(); i++) {
        mod *= 256;
        mod += base256[i];
        if (tRes.size() > 0 || mod >= 10) tRes.push_back(mod / 10);
        mod = mod % 10;
      }
      rTemp.push_back(mod);
      base256 = std::move(tRes);
    }

    for (Nb::const_reverse_iterator it = rTemp.rbegin();
         it != rTemp.rend();
         ++it) {
      result.push_back(*it + 48);
    }
  }

  if (nbDecimals > 0) {

    result.push_back('.');
    size_t max = std::min(N, p_b.size());
    Nb base256(p_b.m_nb.begin(), p_b.m_nb.begin() + max);

    for (size_t i = 0; i < nbDecimals; i++) {
      int retenue = 0;
      Nb rTemp;
      for (size_t i = 0; i < max; i++) {
        retenue += base256[i] * 10;
        rTemp.push_back(retenue % 256);
        retenue = retenue / 256;
      }
      result.push_back(retenue + 48);
      base256 = std::move(rTemp);
    }
  }

  return result;
}


template<size_t N>
void BigNum<N>::round()
{
  if (N == 0) return;
  if (size() < N-1) {
    m_nb.clear();
    return;
  }

  int retenue = (128 + m_nb[N-1]) / 256;
  for (size_t i = N; i < size(); i++) {
    retenue += m_nb[i];
    m_nb[i] = retenue % 256;
    retenue /= 256;
  }
  for (size_t i = 0; i < N; i++) {
    m_nb[i] = 0;
  }
}

template<size_t N>
BigNum<N>& BigNum<N>::operator+=(const BigNum<N>& other)
{
  if (m_isNegative != other.m_isNegative) {
    BigNum<N> temp = other;
    temp.m_isNegative = !other.m_isNegative;
    *this -= temp;
    return *this;
  }

  int retenue = 0;
  size_t max = std::max(size(), other.size());
  for (size_t i = 0; i < max; i++) {
    if (i < size()) retenue += m_nb[i];
    if (i < other.size()) retenue += other.m_nb[i];

    if (i < size()) m_nb[i] = retenue % 256;
    else m_nb.push_back(retenue % 256);
    retenue /= 256;
  }

  if (retenue != 0) m_nb.push_back(retenue);

  return *this;
}

template<size_t N>
BigNum<N>& BigNum<N>::operator-=(const BigNum<N>& other)
{
  if (m_isNegative != other.m_isNegative) {
    BigNum<N> temp = other;
    temp.m_isNegative = !other.m_isNegative;
    *this += temp;
    return *this;
  }

  BigNum<N> temp;
  if (*this < other ^ m_isNegative) {
    temp = *this;
    *this = other;
    m_isNegative = !m_isNegative;
  } else {
    temp = other;
  }

  for (size_t i = 0; i < temp.size(); i++) {
    if (m_nb[i] < temp.m_nb[i]) {
      m_nb[i] += (256 - temp.m_nb[i]);
      size_t j = i + 1;
      while (m_nb[j] == 0) {
        m_nb[j] = 255;
        j++;
      }
      m_nb[j] -= 1;
    } else {
      m_nb[i] -= temp.m_nb[i];
    }
  }

  normalize();

  return *this;
}

template<size_t N>
BigNum<N>& BigNum<N>::operator*=(const BigNum<N>& other) {

  m_isNegative = m_isNegative ^ other.m_isNegative;

  Nb result;

  for (size_t j = 0; j < other.size(); j++) {

    Nb rTemp(j, 0);
    int temp = 0;
    for (size_t i = 0; i < size(); i++) {
      temp += (m_nb[i] * other.m_nb[j]);
      rTemp.push_back(temp % 256);
      temp /= 256;
    }
    if (temp > 0) rTemp.push_back(temp);

    int retenue = 0;
    size_t max = std::max(result.size(), rTemp.size());
    for (size_t i = 0; i < max; i++) {
      if (i < result.size()) retenue += result[i];
      if (i < rTemp.size()) retenue += rTemp[i];

      if (i < result.size()) result[i] = retenue % 256;
      else result.push_back(retenue % 256);
      retenue /= 256;
    }

    if (retenue != 0) result.push_back(retenue);
  }

  if (result.size() > N) m_nb = Nb(result.begin() + N, result.end());
  else m_nb.clear();

  normalize();

  return *this;
}

template<size_t N>
BigNum<N>& BigNum<N>::operator/=(const BigNum<N>& other)
{
  if (other == ZERO) {
    throw "Division by zero";
  }

  m_isNegative = m_isNegative ^ other.m_isNegative;

  m_nb.insert(m_nb.begin(), N, 0);
  const Nb& div = other.m_nb;

  // Some useful intermediate results for long division
  std::vector<Nb> mult_res(8, div);
  for (size_t i = 1; i < 8; i++) {
    size_t mult = 1 << i;
    Nb& nb = mult_res[i];
    int retenue = 0;
    for (size_t j = 0; j < nb.size(); j++) {
      retenue += nb[j] * mult;
      nb[j] = retenue % 256;
      retenue /= 256;
    }
    if (retenue > 0) {
      nb.push_back(retenue);
    }
  }


  Nb temp;

  auto temp_compare = [&](const Nb& nb)
    {
     if (temp.size() < nb.size()) return -1;
     if (temp.size() > nb.size()) return 1;

     for (size_t i = 0; i < temp.size(); i++) {
       if (temp[i] < nb[nb.size() - 1 - i]) return -1;
       if (temp[i] > nb[nb.size() - 1 - i]) return 1;
     }

     return 0;
    };

  auto temp_minus = [&](const Nb& nb)
    {

     for (size_t i = 0; i < nb.size(); i++)
       {
        if (temp[temp.size() - i - 1] < nb[i])
          {
           temp[temp.size() - i - 1] += (256 - nb[i]);
           size_t j = i + 1;
           while (temp[temp.size() - j - 1] == 0)
             {
               temp[temp.size() - j - 1] = 255;
               j++;
             }
           temp[temp.size() - j - 1] -= 1;
          }
        else
          {
           temp[temp.size() - i - 1] -= nb[i];
          }
       }
     Nb::iterator it = temp.begin();
     while (it != temp.end() && *it == 0) {
       ++it;
     }
     temp = Nb(it, temp.end());
    };


  // Long division :/
  for (size_t j = 0; j < m_nb.size(); j++) {
    temp.push_back(m_nb[m_nb.size()-j-1]);

    Digit result = 0;
    for (size_t i = 8; i > 0; i--) {
      if (temp_compare(mult_res[i-1]) >= 0) {
        result += (1 << (i-1));
        temp_minus(mult_res[i-1]);
      }
    }
    m_nb[m_nb.size()-j-1] = result;
  }

  normalize();
  return *this;
}

template<size_t N>
bool BigNum<N>::operator<(const BigNum<N>& other) const
{
  if (m_isNegative != other.m_isNegative) {
    return m_isNegative;
  }

  if (size() < other.size()) return !m_isNegative;
  if (size() > other.size()) return m_isNegative;

  for (size_t i = 0; i < size(); i++) {
    if (m_nb[size() - 1 - i] < other.m_nb[size() - 1 - i]) return !m_isNegative;
    if (m_nb[size() - 1 - i] > other.m_nb[size() - 1 - i]) return m_isNegative;
  }

  return false;
}

template<size_t N>
bool BigNum<N>::operator==(const BigNum<N>& o) const
  {
   return m_isNegative == o.m_isNegative &&
   m_nb == o.m_nb;
  }

template<size_t N>
std::ostream& operator<<(std::ostream& os, const BigNum<N>& b)
{
 os << BigNum<N>::ToString(b, 3);
 return os;
}

