#ifndef CVRP_CONSTANTS_H
#define CVRP_CONSTANTS_H

namespace msi::cvrp {

constexpr int g_iterations = 8;
constexpr int g_depot_id = 0;
constexpr int g_n_ants = 50;
constexpr int g_n_tours = 5;
constexpr int g_n_passes = 50;
constexpr int g_init_capacity = 160;
constexpr double g_alpha = 1.0;
constexpr double g_beta = 1.0;
constexpr double g_evapor = 0.95;

}

#endif//CVRP_CONSTANTS_H
