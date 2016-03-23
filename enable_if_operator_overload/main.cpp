#include <iostream>
#include <numeric>      // std::iota
#include <vector>
#include <type_traits>  // std::enable_if_t - C++14

template <typename T, typename _T = T, typename = typename std::enable_if_t<!std::is_pod<T>::value>>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
  std::cout << "non POD version\n";
  for (const auto& el : v) os << el << ' ';
  os << '\n';
  return os;
}

template <typename T, typename = typename std::enable_if_t<std::is_pod<T>::value>>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
  std::cout << "POD version\n";
  for (const auto el : v) os << el << ' ';
  os << '\n';
  return os;
}

struct X
{
  int s;
  X(int i = 0) : s(i){};  // to make X non pod
};

std::ostream& operator<<(std::ostream& os, const X& x)
{
  os << x.s;
  return os;
}

int main()
{
  constexpr int count = 50;
  std::vector<int> v(count);
  std::iota(v.begin(), v.end(), 0);
  std::cout << v;

  std::vector<X> vX(count);
  std::iota(vX.begin(), vX.end(), 0);
  std::cout << vX;
}
