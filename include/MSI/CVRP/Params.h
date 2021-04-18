#ifndef CVRP_PARAMS_H
#define CVRP_PARAMS_H
#include <cstdint>
#include <vector>

namespace msi::cvrp {

struct Params {

   std::size_t iterations = 10000;//180
   std::size_t depot_id = 0;
   std::size_t vehicle_count = 20;
   std::size_t tour_count = 5;
   std::size_t initial_capacity = 160;
   std::vector<double> alpha{.714, .1726, .05633};
   std::vector<double> beta{3.2839, 2.62934, .005186};
   std::vector<double> evaporation_rate{.988222, .94783, .005589};
   std::size_t polynomial_degree = 2;

   std::size_t current_iteration = 0;
};

}// namespace msi::cvrp

#endif//CVRP_PARAMS_H
