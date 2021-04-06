#ifndef CVRP_IRANDOMGENERATOR_H
#define CVRP_IRANDOMGENERATOR_H
#include <stdlib.h>

namespace msi::util {

class IRandomGenerator {
 public:
   [[nodiscard]] virtual double next_double(double max) noexcept = 0;
   [[nodiscard]] virtual int next_int(int max) noexcept = 0;
};



class Random : public IRandomGenerator {
 public:
   double next_double(double max) noexcept override {
      return static_cast<double>(std::rand() % 10000) * max / 10000.0;
   }

   int next_int(int max) noexcept override {
      return std::rand() % max;
   }
};

}// class IRandomGenerator

#endif//CVRP_IRANDOMGENERATOR_H
