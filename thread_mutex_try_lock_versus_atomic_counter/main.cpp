#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <chrono>
#include <atomic>

std::atomic<u_int64_t> counter_atomic (0); // atomic counter
u_int64_t              counter        (0); // counter locked with mutex
std::mutex mtx;                            // locks access to counter
u_int64_t              unsync_counter (0); // non-atomic counter

void attempt_10k_increases_mutex () {
  for (u_int32_t i=0; i<1000000; ++i) {
    while (!mtx.try_lock()) {   // only increase if currently not locked
      ++unsync_counter;
    }
      ++counter;
      mtx.unlock();
  }
}

void attempt_10k_increases_atomic () {
  for (u_int32_t i=0; i<1000000; ++i) {
      ++counter_atomic;
      mtx.unlock();
  }
}

double p(std::chrono::duration<double, std::milli> duration)
{
  return duration.count();
}

int main ()
{
  constexpr uint8_t num = 5;

  auto start_atomic = std::chrono::steady_clock::now();
  std::thread threads_atomic[num];
  for (int i=0; i<num; ++i)
    threads_atomic[i] = std::thread(attempt_10k_increases_atomic);
  for (auto& th : threads_atomic) th.join();
  std::cout << "atomic version took "
            << p(std::chrono::steady_clock::now() - start_atomic) << "ms\n";

  auto start_mutex = std::chrono::steady_clock::now();
  std::thread threads[num];
  for (int i=0; i<num; ++i)
    threads[i] = std::thread(attempt_10k_increases_mutex);
  for (auto& th : threads) th.join();
  std::cout << "mutex locked version took "
            << p(std::chrono::steady_clock::now() - start_mutex) << "ms\n";

  std::cout << counter_atomic << " successful increases of the counter(synced with atomic).\n";
  std::cout << counter << " successful increases of the counter(locked with mutex).\n";
  std::cout << unsync_counter << " lock failures on mutex\n";
}
