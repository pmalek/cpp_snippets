#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>

template <typename TimeT = std::chrono::milliseconds>
struct measure
{
  template <typename F, typename... Args>
  static typename TimeT::rep execution(F func, Args&&... args)
  {
    using namespace std::chrono;

    auto start = system_clock::now();

    // Now call the function with all the parameters you need.
    func(std::forward<Args>(args)...);

    auto duration = duration_cast<TimeT>(system_clock::now() - start);

    return duration.count();
  }
};

void myquicksort(std::vector<int>& A, const int left, const int right)
{
  int i = left;
  int j = right;
  int pivot = A[left + ((right - left) / 2)];

  while( i < j )
  {
    while ( A[i] < pivot )
      ++i;
    while ( A[j] > pivot )
      --j;
     
    if ( i <= j )
    {
      std::swap(A[i], A[j]);
      ++i;
      --j;
    }
  }

  if(left < j)
    myquicksort(A, left, j);
  if(i < right)
    myquicksort(A, i, right);
}

void quick_sort(std::vector<int>& A)
{
  myquicksort(A, 0, A.size());
}

void s(std::vector<int>& v)
{
  std::sort(v.begin(), v.end());
}

int main()
{
  std::vector<int> v(10000000);

  std::mt19937 mersenne_engine;
  std::uniform_int_distribution<int> dist(1, 100);
  auto gen = std::bind(dist, mersenne_engine);
  std::generate(v.begin(), v.end(), gen);
  
  auto v2 = v; // copy to work on the same data

  std::cout << "quicksort : \t" << measure<>::execution(quick_sort, v) << "ms" << '\n';
  std::cout << "std::sort : \t" << measure<>::execution(s, v2) << "ms" << '\n';
}
