#include <thread>
#include <vector>
#include "thread_safe_hashmap.h"

int main()
{
  const auto concurrency         = std::thread::hardware_concurrency();
  constexpr auto COUNT           = 1000000;
  constexpr auto CHANGE_INERVAL1 = 1000;
  constexpr auto CHANGE_INERVAL2 = 1330;

  thread_safe_hashmap<std::string, long unsigned> map;

  std::vector<std::thread> vt;
  vt.reserve(concurrency);

  auto fun = [&map](const int interval) {
    for (int i = 0; i < COUNT; ++i)
    {
      if (i % interval == 0) map.add_or_update_mapping("key2", i + 13);
      std::cout << "thread: " << std::this_thread::get_id() << ' ' << map.value_for("key2") << '\n';
    }
  };

  vt.emplace_back(fun, CHANGE_INERVAL1);
  vt.emplace_back(fun, CHANGE_INERVAL2);

  for (auto& v : vt) v.join();

  std::cout << map;
}
