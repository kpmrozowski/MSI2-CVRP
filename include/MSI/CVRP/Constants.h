#ifndef CVRP_CONSTANTS_H
#define CVRP_CONSTANTS_H

namespace msi::cvrp {
/* 
1	##########	694.527
7	##########	671.523
79	##########	651.303
95	##########	643.040
108	##########	637.868
112	##########	629.656
132	##########	596.047
149	##########	595.643
317	##########	565.568
*/

constexpr int g_iterations = 318;
constexpr int g_depot_id = 0;
constexpr int g_n_ants = 2;
constexpr int g_n_tours = 5;
constexpr int g_n_passes = 50;
constexpr int g_init_capacity = 160;
constexpr double g_alpha = 0.5;
constexpr double g_alpha_max = 1.0;
constexpr double g_beta = 4.0;
constexpr double g_evapor = 0.95;

}// namespace msi::cvrp

#endif//CVRP_CONSTANTS_H
