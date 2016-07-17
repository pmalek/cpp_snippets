#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include <boost/thread/future.hpp>

/*
 * There is a problem with contention in this snippet:
 * when the number of files to be processed (vector vs), greatly exceeds the number
 * of cores available on the system then we can observe contention and context switching
 * on the CPU. There is no trivial solution for this.
 * Few ideas:
 * - have 2 vectors, one with already running boost::async's and one with deferred ones and as you
 * 'pop'
 *   the ready one (e.g. with .get() from boost::future) you go and .get() the deferred on
 * - have a synchronized queue for tasks and somehow manage it(?) (would it be feasible to have more
 *   than on acive task on this sort of queue?)
 */

using vbf = std::vector<boost::future<std::pair<std::string, int>>>;

void processFile(vbf& v, const std::string& f, int offset)
{
  v.emplace_back(boost::async([offset, &f]() {
    std::cout << "Starting " << f << '\n';
    std::ofstream ofs(f);
    const int max = offset * 1000000;
    for (int i = 0; i < max; ++i) ofs << i;
    return std::make_pair(f, max);
  }));
}

void printProgress(vbf& tasks)
{
  auto it = boost::wait_for_any(tasks.begin(), tasks.end());
  while (it != tasks.end())
  {
    const auto res = it->get();
    std::cout << "processed file \"" << res.first << "\" with " << res.second << " records\n";
    tasks.erase(it);
    it = boost::wait_for_any(tasks.begin(), tasks.end());
  }
}

int main()
{
  std::vector<std::string> vs{
      "file1",  "file2",  "file3",  "file4",  "file5",  "file6",  "file7",  "file8",  "file9",
      "file10", "file11", "file12", "file13", "file14", "file15", "file16", "file17", "file18",
      "file19", "file20", "file21", "file22", "file23", "file24", "file25", "file26", "file27",
      "file28", "file29", "file30", "file31", "file32", "file33", "file34", "file35", "file36",
      "file37", "file38", "file39", "file40", "file41", "file42", "file43", "file44", "file45",
      "file46", "file47", "file48", "file49", "file50"};

  vbf v;
  v.reserve(vs.size());

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 10);

  for (const auto& f : vs) processFile(v, f, dis(gen));

  printProgress(v);
}
