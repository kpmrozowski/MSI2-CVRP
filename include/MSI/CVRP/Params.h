#ifndef CVRP_PARAMS_H
#define CVRP_PARAMS_H
#include <cstdint>

namespace msi::cvrp {

struct Params {
   std::size_t iterations = 8;
   std::size_t depot_id = 0;
   std::size_t vehicle_count = 10;
   std::size_t tour_count = 5;
   std::size_t pass_count = 50;
   std::size_t initial_capacity = 160;
   double alpha_initial = 1.5;//1.50353;
   double beta_initial = 3.6;//3.23893608;
   double evaporation_rate_initial = 0.97;//0.956;
   double alpha_final = 1.5;//1.50353;
   double beta_final = 3.6;//3.23893608;
   double evaporation_rate_final = 0.97;//0.956;
   std::size_t current_iteration = 0;
};

}

#endif//CVRP_PARAMS_H
