#include <iostream>
#include <vector>

#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/iterator_range_core.hpp>

template <typename Predicate, typename Container>
auto make_filter_iterator_range(Predicate&& p, Container&& c) {
  return boost::make_iterator_range(boost::make_filter_iterator(p, c.cbegin()),
                                    boost::make_filter_iterator(p, c.cend()));
}

int main() {
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto isOdd = [](const auto& a) { return a & 1; };

  std::cout << "boost::adaptors::filter\n";
  for(const auto& a : boost::adaptors::filter(v, isOdd)) std::cout << "x=" << a << '\n';

  std::cout << '\n';

  std::cout << "boost::adaptors::filtered\n";
  for(const auto& a : v | boost::adaptors::filtered(isOdd)) std::cout << "x=" << a << '\n';

  std::cout << '\n';

  std::cout << "boost::make_filter_iterator\n";
  auto it_filt = boost::make_filter_iterator(isOdd, v.begin());
  auto it_filt_end = boost::make_filter_iterator(isOdd, v.end());
  for(const auto it : boost::make_iterator_range(it_filt, it_filt_end)) std::cout << "x=" << it << '\n';

  std::cout << '\n';

  std::cout << "make_filter_iterator_range\n";
  for(const auto it : make_filter_iterator_range(isOdd, v)) std::cout << "x=" << it << '\n';
}
