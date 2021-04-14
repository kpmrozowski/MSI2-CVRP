#ifndef CVRP_EVOLUTION_H
#define CVRP_EVOLUTION_H
#include "../Util/IRandomGenerator.h"
#include <MSI/CVRP/Tour.h>
#include <functional>
#include <future>
#include <numeric>
#include <unordered_map>

namespace msi::evolution {

struct Range {
   double min, max;
};

struct Variables {
   double alpha_initial;
   double beta_initial;
   double evaporation_rate_initial;
   double alpha_final;
   double beta_final;
   double evaporation_rate_final;
};

struct Constraint {
   Range alpha_initial;
   Range beta_initial;
   Range evaporation_rate_initial;
   Range alpha_final;
   Range beta_final;
   Range evaporation_rate_final;
};

struct Params {
   std::size_t population_size;
   std::size_t generations_count;
   double mutation_chance;
   double cross_chance;
   double mutation_rate;
   double optimal_fitness;
};

using ObjectiveFunction = std::function<double(const Variables &)>;

std::pair<double, Variables> FindOptimal(util::IRandomGenerator &rand, const ObjectiveFunction &objective_function, Params params, Constraint constraint);

}// namespace msi::evolution

#endif//CVRP_EVOLUTION_H
