#include <iostream>
#include <utility>
#include <array>

template <typename T, T... vals>
constexpr auto make_init_array(std::integer_sequence<T, vals...>)
{
  return std::array<T, sizeof...(vals)>{{vals...}};
}

template <size_t N>
constexpr auto make_array()
{
  return make_init_array(std::make_index_sequence<N>{});
}

int main()
{
  auto a = make_array<100>();
  for( auto i : a )
  {
    std::cout << i << std::endl;
  }
}
