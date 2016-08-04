#include <iostream>
#include <cassert>
#include <bitset>
#include <tuple>

constexpr size_t COUNT = 1024; // bits in numbers

std::bitset<COUNT> add(std::bitset<COUNT> a, std::bitset<COUNT> b)
{
  while((a&b).count())
  {
    //a = a ^ b;
    //b = ((a & b) << 1);
    std::tie(a,b) = std::make_tuple(a^b,((a & b) << 1));
  }

  return a | b;
}

int main()
{
  {
    std::bitset<COUNT> b1{8762};
    std::bitset<COUNT> b2{128937};
    std::bitset<COUNT> b3 = add(b1, b2);
    assert(b3.to_ulong() == 137699);
  }
  {
    std::bitset<COUNT> b1{47};
    std::bitset<COUNT> b2{34};
    std::bitset<COUNT> b3 = add(b1, b2);
    assert(b3.to_ulong() == 81);
  }
}
