#ifndef CVRP_EVOLUTION_H
#define CVRP_EVOLUTION_H
#include "../Util/IRandomGenerator.h"
#include <functional>
#include <numeric>
#include <unordered_map>

namespace msi::evolution {

struct Range {
   double min, max;
};

struct Variables {
   double alpha_initial, beta_initial, evaporation_rate_initial, alpha_final, beta_final, evaporation_rate_final;
};

struct Constraint {
   Range alpha_initial;
   Range beta_initial;
   Range evaporation_rate_initial;
   Range alpha_final;
   Range beta_final;
   Range evaporation_rate_final;
};

using ObjectiveFunction = std::function<double(const Variables &)>;

Variables FindOptimal(util::IRandomGenerator &rand, const ObjectiveFunction&objective_function, Constraint constraint);

}// namespace msi::evolution

#endif//CVRP_EVOLUTION_H
