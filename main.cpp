#include <chrono>
#include <iostream>
#include <cstddef>
#include <vector>
#include <future>

namespace kondrat
{
  class Clicker
  {
    public:
      Clicker():
        start_(std::chrono::high_resolution_clock::now())
      {}
      double millisec() const
      {
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto t = high_resolution_clock::now();
        return duration_cast< milliseconds >(t - start_).count();
      }
    private:
      std::chrono::time_point< std::chrono::steady_clock > start_;
  };
}

namespace knd = kondrat;
using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

value_t sumBlock(const data_t & value, size_t begin, size_t end)
{
  value_t sum = 0;

  for (size_t i = begin; i < end; ++i)
  {
    sum += value[i];
  }

  return sum;
}

int main(int argc, char ** argv)
{
  constexpr size_t size{1'000'000'000};
  double init{0}, total{0};
  value_t sum{0};
  {
    if (argc != 2)
    {
      std::cout << "Usage: " << argv[0] << " <threads>\n";
      return 1;
    }

    size_t threads = 0;

    try
    {
      threads = std::stoull(argv[1]);
    }
    catch (...)
    {
      std::cout << "Threads count must be a positive number\n";
      return 1;
    }

    if (threads == 0)
    {
      std::cout << "Invalid threads count\n";
      return 1;
    }

    knd::Clicker cl;
    data_t values(size, 1);
    init = cl.millisec();

    std::vector<std::future<value_t>> futures;

    for (size_t i = 0; i < threads; ++i)
    {
      size_t begin = i * size / threads;
      size_t end = (i + 1) * size / threads;

      futures.push_back(std::async(std::launch::async, sumBlock, std::cref(values), begin, end));
    }


    for (size_t i = 0; i < threads; ++i)
    {
      sum += futures[i].get();
    }

    total = cl.millisec();
  }

  std::cout << "Init: " << init << "\n";
  std::cout << "Total: " << total << "\n";
  std::cout << "Total - Init: " << total - init << "\n";

}
