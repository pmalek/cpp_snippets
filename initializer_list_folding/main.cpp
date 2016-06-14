/*
 * Based on article http://articles.emptycrate.com/2016/05/14/folds_in_cpp11_ish.html
 * C++14 due to std::common_type_t and auto return type without ->
 */

#include <functional>
#include <utility>
#include <type_traits>
#include <iostream>

template <typename U, typename... V>
auto min(const U &u, const V &... v)
{
  using rettype = typename std::common_type_t<U, V...>;
  rettype result = static_cast<rettype>(u);
  (void)std::initializer_list<int>{((v < result) ? (result = static_cast<rettype>(v), 0) : 0)...};
  return result;
}

template <typename U, typename... V>
auto max(const U &u, const V &... v)
{
  using rettype = typename std::common_type_t<U, V...>;
  rettype result = static_cast<rettype>(u);
  (void)std::initializer_list<int>{((v > result) ? (result = static_cast<rettype>(v), 0) : 0)...};
  return result;
}

template <typename... V>
auto sum_all(const V &... v)
{
  using rettype = typename std::common_type_t<V...>;
  rettype result{};
  (void)std::initializer_list<int>{(result += v, 0)...};
  return result;
}

int main()
{
  std::cout << min(1, 2, 3.4, -1.2, 8.9) << '\n';
  std::cout << max(1, 2, 3.4, -1.2, 8.9) << '\n';
  std::cout << sum_all(1, 2) << '\n';
  std::cout << sum_all(1, 2, 3) << '\n';
}
