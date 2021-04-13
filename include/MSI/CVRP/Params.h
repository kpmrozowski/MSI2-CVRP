#ifndef CVRP_PARAMS_H
#define CVRP_PARAMS_H
#include <cstdint>

namespace msi::cvrp {

struct Params {
   std::size_t iterations = 30;
   std::size_t depot_id = 0;
   std::size_t vehicle_count = 10;
   std::size_t tour_count = 5;
   std::size_t pass_count = 50;
   std::size_t initial_capacity = 160;
   double alpha_initial = 1.892192;//1.50353;
   double beta_initial = 4.0646515;//3.23893608;
   double evaporation_rate_initial = 0.971003085;//0.956;
   double alpha_final = 1.513364;//1.50353;
   double beta_final = 3.3867969;//3.23893608;
   double evaporation_rate_final = 0.9811404145;//0.956;
   std::size_t current_iteration = 0;
};

//  681 605 619 567 627 613 615 633 642 631 635 630 T 567.3949589046433
//  final result:
//  alpha_initial: 1.892192
//  beta_initial: 4.0646515
//  evaporation_initial: 0.971003085
//  alpha_final: 1.513364
//  beta_final: 3.3867969
//  evaporation_final: 0.9811404145

// final result:
//   alpha_initial: 1.7406444073304124
//   beta_initial: 9
//   evaporation_initial: 0.85
//   alpha_final: 1.2136495740909226
//   beta_final: 0.01
//   evaporation_final: 0.5079737299999999
//   Fitness: 573.6677372822943
// blind luck
}

#endif//CVRP_PARAMS_H
