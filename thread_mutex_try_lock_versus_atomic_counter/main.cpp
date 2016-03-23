#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <chrono>
#include <atomic>
#include <vector>

std::atomic<u_int64_t> counter_atomic (0); // atomic counter
u_int64_t              counter        (0); // counter locked with mutex
std::mutex             mtx;                // locks access to counter
u_int64_t              unsync_counter (0); // non-atomic counter

void attempt_10k_increases_mutex () {
  for (u_int32_t i=0; i<1000000; ++i) {
    std::lock_guard<std::mutex> lock(mtx);
    ++counter;
  }
}

void attempt_10k_increases_atomic () {
  for (u_int32_t i=0; i<1000000; ++i) ++counter_atomic;
}

double p(std::chrono::duration<double, std::milli> duration)
{
  return duration.count();
}

int main ()
{
  const uint8_t num = std::thread::hardware_concurrency();

  auto start_atomic = std::chrono::steady_clock::now();

  std::vector<std::thread> threads_atomic;
  threads_atomic.reserve(num);
  for (int i=0; i<num; ++i) threads_atomic.emplace_back(attempt_10k_increases_atomic);
  for (auto& th : threads_atomic) th.join();
  std::cout << "atomic version took "
            << p(std::chrono::steady_clock::now() - start_atomic) << "ms\n";

  auto start_mutex = std::chrono::steady_clock::now();
  std::vector<std::thread> threads;
  threads.reserve(num);
  for (int i=0; i<num; ++i) threads.emplace_back(attempt_10k_increases_mutex);
  for (auto& th : threads) th.join();
  std::cout << "mutex locked version took "
            << p(std::chrono::steady_clock::now() - start_mutex) << "ms\n";

  std::cout << counter_atomic << " successful increases of the counter(synced with atomic).\n";
  std::cout << counter << " successful increases of the counter(locked with mutex).\n";
  std::cout << unsync_counter << " lock failures on mutex\n";
}
