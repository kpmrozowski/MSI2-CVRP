#include <MSI/Evolution/Evolution.h>

namespace msi::evolution {

constexpr auto g_population_size = 12;
constexpr auto g_generations_count = 12;
constexpr auto g_mutation_chance = 0.8;
constexpr auto g_mutation_rate = 0.95;
constexpr auto g_cross = 0.8;
constexpr auto g_optimal_fitness = 521.;

constexpr double mutate(double min, double max, double a, double b, double c) {
   return std::clamp(a + g_mutation_chance * (b - c), min, max);
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
   auto it_optimum = std::min(fitness.begin(), fitness.end());
   std::vector<double> fitness_to_normalise(g_population_size);
   double fitness_to_normalise_sum{};
   std::vector<std::pair<double, Variables>> accumulated_fits(g_population_size);
   // std::set<double> selected_fits;
   // std::vector<Variables> selected_vars;
   std::transform(population.begin(), population.end(), fitness.begin(), objective_function);


   for (std::size_t i = 0; i < g_generations_count; ++i) {
      std::vector<Variables> selected;
      std::vector<Variables> crossovers;
      std::vector<Variables> mutants(g_population_size);
      double accumulated_fit{0};
      std::transform(fitness.begin(), fitness.end(), fitness_to_normalise.begin(), [&](double fit) -> double {
         return 1. / (fit - g_optimal_fitness);
      });
      fitness_to_normalise_sum = std::accumulate(fitness_to_normalise.begin(), fitness_to_normalise.end(), decltype(fitness_to_normalise)::value_type(0));
      std::transform(fitness.begin(), fitness.end(), population.begin(), accumulated_fits.begin(), [&fitness_to_normalise_sum, &accumulated_fit](double fit, Variables vars) {
         fit /= fitness_to_normalise_sum;
         accumulated_fit += fit;
         return std::make_pair(accumulated_fit, vars);
      });
      // std::sort(accumulated_fits.begin(), accumulated_fits.end(), [](auto &left, auto &right) {
      //    return left.first < right.first;
      // });

      // selection
      while (selected.size() < g_population_size * 2) {
         double value = rand.next_double(1.0);
         for (auto fnv : accumulated_fits) {
            value -= fnv.first;
            if (value <= 0) {
               // Losowanie bez zwracania
               // if(selected_map.find(fnv.first) == selected_map.end())
               //    continue;
               selected.push_back(fnv.second);
               break;
            }
         }
      }
      // crossover
      std::size_t crossovers_count{rand.next_int(g_population_size)};
      std::size_t omega{rand.next_double(1.0)};// overrelaxation coefficient
      std::set<std::size_t> parents{};
      while (crossovers.size() < crossovers_count) {
         std::size_t fatherId{rand.next_int(g_population_size)};
         std::size_t motherId{rand.next_int(g_population_size)};
         if (parents.find(fatherId) == parents.end() && parents.find(motherId) == parents.end()){
            parents.insert(fatherId);
            parents.insert(motherId);
         }
         else
            continue;
         double alpha_initial, beta_initial, evaporation_rate_initial, alpha_final, beta_final, evaporation_rate_final;
         if (rand.next_double(1.0) < g_cross) {
            alpha_initial = (1 - omega) * selected[motherId].alpha_initial + omega * selected[fatherId].alpha_initial;
         }
         if (rand.next_double(1.0) < g_cross) {
            beta_initial = (1 - omega) * selected[motherId].beta_initial + omega * selected[fatherId].beta_initial;
         }
         if (rand.next_double(1.0) < g_cross) {
            evaporation_rate_initial = (1 - omega) * selected[motherId].evaporation_rate_initial + omega * selected[fatherId].evaporation_rate_initial;
         }
         if (rand.next_double(1.0) < g_cross) {
            alpha_final = (1 - omega) * selected[motherId].alpha_final + omega * selected[fatherId].alpha_final;
         }
         if (rand.next_double(1.0) < g_cross) {
            beta_final = (1 - omega) * selected[motherId].beta_final + omega * selected[fatherId].beta_final;
         }
         if (rand.next_double(1.0) < g_cross) {
            evaporation_rate_final = (1 - omega) * selected[motherId].evaporation_rate_final + omega * selected[fatherId].evaporation_rate_final;
         }
         crossovers.emplace_back(Variables{alpha_initial, beta_initial, evaporation_rate_initial, alpha_final, beta_final, evaporation_rate_final});
      }
      for (std::size_t p = crossovers.size(); p < g_population_size; p++) {
         std::size_t singleId{rand.next_int(g_population_size)};
         while (parents.find(singleId) != parents.end())
            parents.insert(singleId);
         crossovers.emplace_back(Variables{
                 selected[singleId].alpha_initial,
                 selected[singleId].beta_initial,
                 selected[singleId].evaporation_rate_initial,
                 selected[singleId].alpha_final,
                 selected[singleId].beta_final,
                 selected[singleId].evaporation_rate_final});
      }
      // mutation
      std::transform(crossovers.begin(), crossovers.end(), population.begin(), [&rand](Variables &crossover) {
         double alpha_initial, beta_initial, evaporation_rate_initial, alpha_final, beta_final, evaporation_rate_final;
         if (rand.next_double(1.0) < g_mutation_chance) {
            alpha_initial = crossover.alpha_initial*rand.next_double(g_mutation_rate, 1 - g_mutation_rate);
         }
         if (rand.next_double(1.0) < g_mutation_chance) {
            beta_initial = crossover.beta_initial*rand.next_double(g_mutation_rate, 1 - g_mutation_rate);
         }
         if (rand.next_double(1.0) < g_mutation_chance) {
            evaporation_rate_initial = crossover.evaporation_rate_initial*rand.next_double(g_mutation_rate, 1 - g_mutation_rate);
         }
         if (rand.next_double(1.0) < g_mutation_chance) {
            alpha_final = crossover.alpha_final*rand.next_double(g_mutation_rate, 1 - g_mutation_rate);
         }
         if (rand.next_double(1.0) < g_mutation_chance) {
            beta_final = crossover.beta_final*rand.next_double(g_mutation_rate, 1 - g_mutation_rate);
         }
         if (rand.next_double(1.0) < g_mutation_chance) {
            evaporation_rate_final = crossover.evaporation_rate_final*rand.next_double(g_mutation_rate, 1 - g_mutation_rate);
         }

         return Variables{alpha_initial, beta_initial, evaporation_rate_initial, alpha_final, beta_final, evaporation_rate_final};
      });
      std::transform(population.begin(), population.end(), fitness.begin(), objective_function);
      for (std::size_t p = 0; p < g_population_size; ++p)
         if (fitness[p] < *it_optimum)
            it_optimum = fitness.begin() + p;
   }


   return population[it_optimum - fitness.begin()];
}
}// namespace msi::evolution