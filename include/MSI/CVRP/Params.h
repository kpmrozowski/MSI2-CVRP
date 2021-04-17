#ifndef CVRP_PARAMS_H
#define CVRP_PARAMS_H
#include <cstdint>
#include <vector>

namespace msi::cvrp {

struct Params {

   std::size_t iterations = 120;//120
   std::size_t depot_id = 0;
   std::size_t vehicle_count = 20;
   std::size_t tour_count = 5;
   std::size_t initial_capacity = 160;
   std::vector<double> alpha{.5, .5, .001};
   std::vector<double> beta{3.6, 3.6, .00001};
   std::vector<double> evaporation_rate{.4, .4, .001};
   std::size_t polynomial_degree = 2;

   std::size_t current_iteration = 0;
};

}

#endif//CVRP_PARAMS_H
