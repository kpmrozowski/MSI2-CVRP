#include <MSI/Evolution/Evolution.h>

namespace msi::evolution {

constexpr auto g_population_size = 12;
constexpr auto g_iteration_count = 12;
constexpr auto g_mutation = 0.8;
constexpr auto g_cross = 0.2;

constexpr double mutate(double min, double max, double a, double b, double c) {
   return std::clamp(a + g_mutation * (b - c), min, max);
}

Variables FindOptimal(util::IRandomGenerator &rand, const ObjectiveFunction &objective_function, const Constraint constraint) {
   std::vector<Variables> population(g_population_size);
   std::generate(population.begin(), population.end(), [&rand, &constraint]() {
      return Variables{
              rand.next_double(constraint.alpha.min, constraint.alpha.max),
              rand.next_double(constraint.beta.min, constraint.beta.max),
              rand.next_double(constraint.evaporation_rate.min, constraint.evaporation_rate.max),
      };
   });

   std::vector<double> fitness(g_population_size);
   std::transform(population.begin(), population.end(), fitness.begin(), objective_function);

   auto it_optimum = std::min(fitness.begin(), fitness.end());

   for (std::size_t i = 0; i < g_iteration_count; ++i) {
      for (std::size_t p = 0; p < g_population_size; ++p) {
         auto chosen = util::choose_n<Variables, 3>(rand, population);
         Variables mutant{
                 mutate(constraint.alpha.min, constraint.alpha.max, chosen[0].alpha, chosen[1].alpha, chosen[2].alpha),
                 mutate(constraint.beta.min, constraint.beta.max, chosen[0].beta, chosen[1].beta, chosen[2].beta),
                 mutate(constraint.evaporation_rate.min, constraint.evaporation_rate.max, chosen[0].evaporation_rate, chosen[1].evaporation_rate, chosen[2].evaporation_rate),
         };
         auto mutant_fitness = objective_function(mutant);

         if (rand.next_double(1.0) < g_cross) {
            mutant.alpha = population[p].alpha;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.beta = population[p].beta;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.evaporation_rate = population[p].evaporation_rate;
         }

         if (mutant_fitness < fitness[p]) {
            fitness[p] = mutant_fitness;
            population[p] = mutant;
            if (mutant_fitness < *it_optimum) {
               it_optimum = fitness.begin() + p;
            }
         }
      }
   }

   return population[it_optimum - fitness.begin()];
}

}// namespace msi::evolution