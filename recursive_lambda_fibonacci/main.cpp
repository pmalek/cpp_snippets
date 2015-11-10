#include <functional>
#include <iostream>

/*
 * Create a recursive lambda that returns a sum of
 * Fibonacci's sequence of 'n' elements
 */
template <typename T>
T fibo(T n)
{
  return [=](T n) {
    std::function<T(T)> recurse = [&](T n){
       return (n<=2)? 1 : recurse(n-1) + recurse(n-2);
    };
    return recurse(n);
  }(n);
}

int main()
{
  std::cout << fibo(10) << std::endl;
}
