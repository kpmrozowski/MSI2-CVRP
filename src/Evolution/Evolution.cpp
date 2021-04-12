#include <MSI/Evolution/Evolution.h>

namespace msi::evolution {

constexpr auto g_population_size = 12;
constexpr auto g_iteration_count = 12;
constexpr auto g_mutation = 0.8;
constexpr auto g_cross = 0.2;
constexpr auto g_omega = 0.5; // overrelaxation coefficient

constexpr double mutate(double min, double max, double a, double b, double c) {
   return std::clamp(a + g_mutation * (b - c), min, max);
}

Variables FindOptimal(util::IRandomGenerator &rand, const ObjectiveFunction &objective_function, const Constraint constraint) {
   std::vector<Variables> population(g_population_size);
   std::generate(population.begin(), population.end(), [&rand, &constraint]() {
      return Variables{
              rand.next_double(constraint.alpha_initial.min, constraint.alpha_initial.max),
              rand.next_double(constraint.beta_initial.min, constraint.beta_initial.max),
              rand.next_double(constraint.evaporation_rate_initial.min, constraint.evaporation_rate_initial.max),
              rand.next_double(constraint.alpha_final.min, constraint.alpha_final.max),
              rand.next_double(constraint.beta_final.min, constraint.beta_final.max),
              rand.next_double(constraint.evaporation_rate_final.min, constraint.evaporation_rate_final.max),
      };
   });

   std::vector<double> fitness(g_population_size);
   std::transform(population.begin(), population.end(), fitness.begin(), objective_function);

   auto it_optimum = std::min(fitness.begin(), fitness.end());

   for (std::size_t i = 0; i < g_iteration_count; ++i) {
      for (std::size_t p = 0; p < g_population_size; ++p) {
         auto chosen = util::choose_n<Variables, 3>(rand, population);
         Variables mutant{
                 mutate(constraint.alpha_initial.min, constraint.alpha_initial.max, chosen[0].alpha_initial, chosen[1].alpha_initial, chosen[2].alpha_initial),
                 mutate(constraint.beta_initial.min, constraint.beta_initial.max, chosen[0].beta_initial, chosen[1].beta_initial, chosen[2].beta_initial),
                 mutate(constraint.evaporation_rate_initial.min, constraint.evaporation_rate_initial.max, chosen[0].evaporation_rate_initial, chosen[1].evaporation_rate_initial, chosen[2].evaporation_rate_initial),
                 mutate(constraint.alpha_final.min, constraint.alpha_final.max, chosen[0].alpha_final, chosen[1].alpha_final, chosen[2].alpha_final),
                 mutate(constraint.beta_final.min, constraint.beta_final.max, chosen[0].beta_final, chosen[1].beta_final, chosen[2].beta_final),
                 mutate(constraint.evaporation_rate_final.min, constraint.evaporation_rate_final.max, chosen[0].evaporation_rate_final, chosen[1].evaporation_rate_final, chosen[2].evaporation_rate_final),
         };

         if (rand.next_double(1.0) < g_cross) {
            mutant.alpha_initial = (1 - g_omega) * mutant.alpha_initial + g_omega * population[p].alpha_initial;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.beta_initial = (1 - g_omega) * mutant.beta_initial + g_omega * population[p].beta_initial;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.evaporation_rate_initial = (1 - g_omega) * mutant.evaporation_rate_initial + g_omega * population[p].evaporation_rate_initial;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.alpha_final = (1 - g_omega) * mutant.alpha_final + g_omega * population[p].alpha_final;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.beta_final = (1 - g_omega) * mutant.beta_final + g_omega * population[p].beta_final;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.evaporation_rate_final = (1 - g_omega) * mutant.evaporation_rate_final + g_omega * population[p].evaporation_rate_final;
         }

         auto mutant_fitness = objective_function(mutant);

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