#include <utility>
#include <iostream>

// variable templates available since gcc 5 (N3651) in c++14
template <size_t n>
constexpr size_t fact{n * fact<n - 1>};
template <>
constexpr size_t fact<0>{1};

// relaxed constexpr functions
// (more than just a return statement)
// since gcc 5 (N3652) in c++14
template <typename T>
constexpr auto factorial(T n)
{
  unsigned long long res{1};
  while (n > 1) res *= n--;
  return res;
}

int main()
{
  static_assert(fact<1> == factorial(1), "this should be equal");
  static_assert(fact<5> == factorial(5), "this should be equal");
  static_assert(fact<10> == factorial(10), "this should be equal");

  std::cout << fact<1> << std::endl;
  std::cout << fact<5> << std::endl;
  std::cout << fact<10> << std::endl;

  std::cout << factorial(1) << std::endl;
  std::cout << factorial(5) << std::endl;
  std::cout << factorial(10) << std::endl;

  return 0;
}
