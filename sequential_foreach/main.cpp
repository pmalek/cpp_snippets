#include <initializer_list>
#include <iostream>
#include <string>

/*
 * somehow this doesn't compile (up to gcc 5.2.1 - later versions?)
 *
 *template<typename Fun, typename...Ts>
 *void sequential_foreach(Fun f, const Ts&... args) {
 *  (void) std::initializer_list<int>{
 *    [&](const auto& arg){f(arg); return 0;}(args)...
 *  };
 *}
 */

template <typename Fun, typename... Ts>
void sequential_foreach(Fun f, const Ts&... args)
{
  /*
   * this auto requires c++14
   */
  auto fun = [&](const auto& arg) {
    f(arg);
    return 0;
  };
  (void)std::initializer_list<int>{fun(args)...};
}

template <typename... Ts>
void print_all(std::ostream& stream, const Ts&... args)
{
  sequential_foreach([&](const auto& arg) { stream << arg; }, args...);
}

int main()
{
  std::string s1("string1\n");
  std::string s2("string2\n");
  print_all(std::cout, s1, s2);
}
