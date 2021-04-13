#ifndef CVRP_IRANDOMGENERATOR_H
#define CVRP_IRANDOMGENERATOR_H
#include <array>
#include <set>
#include <stdlib.h>
#include <vector>

namespace msi::util {

class IRandomGenerator {
 public:
   [[nodiscard]] virtual double next_double(double max) noexcept = 0;
   [[nodiscard]] virtual double next_double(double min, double max) noexcept = 0;
   [[nodiscard]] virtual int next_int(int max) noexcept = 0;
   [[nodiscard]] virtual int next_int(int min, int max) noexcept = 0;
};

class Random : public IRandomGenerator {
 public:
   double next_double(double max) noexcept override {
      return static_cast<double>(std::rand() % 10000) * max / 10000.0;
   }

   double next_double(double min, double max) noexcept override {
      return next_double(max - min) + min;
   }

   int next_int(int max) noexcept override {
      return std::rand() % max;
   }

   int next_int(int min, int max) noexcept override {
      return next_int(max - min) + min;
   }
};

template<typename T, std::size_t n>
std::array<T, n> choose_n(IRandomGenerator &rand, const std::vector<T> &input) {
   std::array<T, n> result;
   std::size_t selected_count{};
   std::set<std::size_t> selected;

   while (selected_count != n) {
      std::size_t chosen;
      std::set<std::size_t>::iterator it;
      do {
         chosen = rand.next_int(input.size());
         it = selected.find(chosen);
      } while (it != selected.end());

      selected.insert(chosen);
      result[selected_count] = input[chosen];
      ++selected_count;
   }

   return result;
}

}// namespace msi::util

#endif//CVRP_IRANDOMGENERATOR_H
