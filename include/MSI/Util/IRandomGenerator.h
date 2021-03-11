#ifndef CVRP_IRANDOMGENERATOR_H
#define CVRP_IRANDOMGENERATOR_H

namespace msi::util {

class IRandomGenerator {
 public:
   [[nodiscard]] virtual double next_double(double max) noexcept = 0;
   [[nodiscard]] virtual int next_int(int max) noexcept = 0;
};

}

#endif//CVRP_IRANDOMGENERATOR_H
