#ifndef CVRP_EVOLUTION_H
#define CVRP_EVOLUTION_H
#include "../Util/IRandomGenerator.h"
#include <MSI/CVRP/Tour.h>
#include <functional>
#include <numeric>
#include <unordered_map>
#include <future>

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

std::pair<double, Variables> FindOptimal(const int g_population_size, const int g_generations_count, const double g_mutation_chance, const double g_cross_chance, const double g_mutation_rate, const double g_optimal_fitness, util::IRandomGenerator &rand, const ObjectiveFunction&objective_function, Constraint constraint);

}// namespace msi::evolution

#endif//CVRP_EVOLUTION_H
