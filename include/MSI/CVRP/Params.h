#ifndef CVRP_PARAMS_H
#define CVRP_PARAMS_H
#include <cstdint>

struct Params {
   std::size_t iterations = 8;
   std::size_t depot_id = 0;
   std::size_t vehicle_count = 50;
   std::size_t tour_count = 5;
   std::size_t pass_count = 50;
   std::size_t initial_capacity = 160;
   double alpha = 1.0;
   double beta = 1.0;
   double evaporation_rate = 0.95;
};

#endif//CVRP_PARAMS_H
