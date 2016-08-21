#include <atomic>
#include <iostream>

std::atomic<std::size_t> TypeCount(0);

template<typename T>
std::size_t getType(){
  static std::size_t id = ++TypeCount;
  return id;
}

struct Foo{};

int main()
{
  std::cout << getType<int>() << std::endl;
  std::cout << getType<int>() << std::endl;
  std::cout << getType<double>() << std::endl;
  std::cout << getType<double>() << std::endl;
  std::cout << getType<double>() << std::endl;
  std::cout << getType<Foo>() << std::endl;
}
