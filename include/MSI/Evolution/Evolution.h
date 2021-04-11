#ifndef CVRP_EVOLUTION_H
#define CVRP_EVOLUTION_H
#include "../Util/IRandomGenerator.h"
#include <functional>

namespace msi::evolution {

struct Range {
   double min, max;
};

struct Variables {
   double alpha, beta, evaporation_rate;
};

struct Constraint {
   Range alpha;
   Range beta;
   Range evaporation_rate;
};

using ObjectiveFunction = std::function<double(const Variables &)>;

Variables FindOptimal(util::IRandomGenerator &rand, const ObjectiveFunction&objective_function, Constraint constraint);

}// namespace msi::evolution

#endif//CVRP_EVOLUTION_H
