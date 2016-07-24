#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

#include <boost/program_options.hpp>

namespace po = boost::program_options;
using FreqMap_t = std::unordered_map<std::string, int>;

struct tokens: std::ctype<char>
{
    tokens(): std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table()
    {
        typedef std::ctype<char> cctype;
        static const cctype::mask *const_rc= cctype::classic_table();

        static cctype::mask rc[cctype::table_size];
        std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

        rc[','] = rc['('] = rc[')'] = rc['&'] = rc[';'] = rc['.'] = rc['['] = rc[']'] =
            rc['<'] = rc['>'] = rc['/'] = std::ctype_base::space;

        return &rc[0];
    }
};

FreqMap_t getFreqMap(const std::string &filename)
{
  FreqMap_t m;
  std::ifstream ifs(filename);
  ifs.imbue(std::locale(std::locale(), new tokens()));
  std::string buff;
  for (std::istream_iterator<std::string> in{ifs}, end; in != end; ++in)
  {
    ++m[*in];
  }
  return m;
}

int getFileNameFromArgs(int argc, char *argv[], boost::program_options::variables_map &vm)
{
  po::options_description desc{"Options"};
  desc.add_options()
    ("help,h", "Help screen")
    ("file,f", po::value<std::string>()->required(), "File for counting")
    ("count,c", po::value<int>()->default_value(10), "Number of most frequent words to print");

  try
  {
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
  }
  catch (const po::error &ex)
  {
    std::cerr << ex.what() << '\n';
    std::cerr << desc << '\n';
    return 1;
  }

  if (vm.count("help"))
  {
    std::cout << desc << '\n';
    return 0;
  }
  if (vm.count("file") && vm.count("count"))
    return 0;
  return 1;
}

int main(int argc, char *argv[])
{
  std::ios_base::sync_with_stdio(0);

  po::variables_map vm;
  if(getFileNameFromArgs(argc, argv, vm) != 0)
    return 1;

  const std::string filename = vm["file"].as<std::string>();

  if (!std::ifstream(filename))
  {
    std::cerr << "File: " << filename << " doesn't exist!" << '\n';
    return 1;
  }

  FreqMap_t map = getFreqMap(filename);
  const int count = std::min(vm["count"].as<int>(), static_cast<int>(map.size()));
  std::vector<std::pair<FreqMap_t::key_type, FreqMap_t::mapped_type>> v{map.cbegin(), map.cend()};

  const auto lastNth = std::next(v.begin(), count);
  auto cmp = [](const auto &p1, const auto &p2)
  {
    return p1.second > p2.second;
  };
  std::nth_element(v.begin(), lastNth, v.end(), cmp);
  std::sort(v.begin(), lastNth, cmp);
  std::cout << "Most " << count << " frequent words in " << filename << '\n';
  for (auto it = v.cbegin(); it != lastNth; ++it)
    std::cout << "'" << it->first << "' - " << it->second << " occurences" << '\n';
}
