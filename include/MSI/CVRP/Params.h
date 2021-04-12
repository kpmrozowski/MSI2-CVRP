#ifndef CVRP_PARAMS_H
#define CVRP_PARAMS_H
#include <cstdint>

namespace msi::cvrp {

struct Params {
   std::size_t iterations = 12;
   std::size_t depot_id = 0;
   std::size_t vehicle_count = 50;
   std::size_t tour_count = 5;
   std::size_t pass_count = 50;
   std::size_t initial_capacity = 160;
   double alpha = 1.50353;
   double beta = 3.23893608;
   double evaporation_rate = 0.956;
};

}

#endif//CVRP_PARAMS_H
