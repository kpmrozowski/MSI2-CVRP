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
   std::vector<double> alpha;
   std::vector<double> beta;
   std::vector<double> evaporation_rate;
};

struct Constraint {
   std::vector<Range> alpha;
   std::vector<Range> beta;
   std::vector<Range> evaporation_rate;
};

struct Params {
   std::size_t population_size;
   std::size_t generations_count;
   double mutation_chance;
   double cross_chance;
   double mutation_rate;
   double optimal_fitness;
   std::size_t polynomial_degree;
};

using ObjectiveFunction = std::function<double(const Variables &)>;

std::pair<double, Variables> FindOptimal(std::vector<std::unique_ptr<msi::cvrp::Tour>> &tours, util::IRandomGenerator &rand, const ObjectiveFunction &objective_function, Params params, Constraint constraint);

}// namespace msi::evolution

#endif//CVRP_EVOLUTION_H
