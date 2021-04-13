#ifndef CVRP_PARAMS_H
#define CVRP_PARAMS_H
#include <cstdint>

namespace msi::cvrp {

struct Params {
   std::size_t iterations = 10;
   std::size_t depot_id = 0;
   std::size_t vehicle_count = 2;
   std::size_t tour_count = 5;
   std::size_t pass_count = 50;
   std::size_t initial_capacity = 160;
   double alpha_initial = 1.7926566875389438;//1.50353;
   double beta_initial = 3.4984326367411205;//3.23893608;
   double evaporation_rate_initial = 0.9681812589486821;//0.956;
   double alpha_final = 1;//1.50353;
   double beta_final = 0.01;//3.23893608;
   double evaporation_rate_final = 0.8116759443374363;//0.956;
   std::size_t current_iteration = 0;
};

}

#endif//CVRP_PARAMS_H
