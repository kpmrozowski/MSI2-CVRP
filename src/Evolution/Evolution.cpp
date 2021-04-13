#include <MSI/Evolution/Evolution.h>
#include <fmt/core.h>

namespace msi::evolution {

constexpr auto g_population_size = 24;
constexpr auto g_generations_count = 10;
constexpr auto g_mutation_chance = 0.8;
constexpr auto g_cross_chance = 0.8;
constexpr auto g_mutation_rate = 0.1;// max change: 10%
constexpr auto g_optimal_fitness = 521.;

std::pair<double, Variables> FindOptimal(util::IRandomGenerator &rand, const ObjectiveFunction &objective_function, const Constraint constraint) {
   std::vector<Variables> population(g_population_size);

   std::generate(population.begin(), population.end(), [&rand, &constraint]() {
      return Variables{
              // initial constraints
              rand.next_double(0.1, 2.0),
              rand.next_double(3.0, 9.0),
              rand.next_double(0.85, 0.999),
              rand.next_double(1.0, 2.0),
              rand.next_double(0.01, 5.0),
              rand.next_double(0.5, 0.9),
      };
   });

   std::vector<std::pair<double, Variables>> fits_vars(g_population_size);
   std::vector<std::vector<std::pair<double, Variables>>> generations(g_generations_count);
   std::vector<double> fitness(g_population_size);
   std::vector<double> fitness_to_normalise(g_population_size);
   double fitness_to_normalise_sum{};
   std::vector<std::pair<double, Variables>> normalised_fits(g_population_size);
   fmt::print("\nEWALUATING {} ANTS\n", g_population_size);


   // MULTITHREADING
   // std::future<double> fitness_threads =
   //         std::async(objective_function, &population[0]);
   // for (std::size_t thread_nr = 0; thread_nr < g_population_size % 12 + 1; thread_nr++)
   // ;
   std::transform(population.begin(), population.end(), fitness.begin(), objective_function);
   fmt::print("EWALUATION FINISHED\n\n");


   auto it_optimum = std::min(fitness.begin(), fitness.end());
   auto it_optimum_population = generations.begin();
   std::transform(fitness.begin(), fitness.end(), population.begin(), fits_vars.begin(), [](double fit, Variables vars) {
      return std::make_pair(fit, vars);
   });


   for (std::size_t i = 0; i < g_generations_count; ++i) {
      generations[i] = fits_vars;
      std::vector<Variables> selected;
      std::vector<Variables> crossovers;
      std::vector<Variables> mutants(g_population_size);
      // double accumulated_fit{0};
      std::transform(fitness.begin(), fitness.end(), fitness_to_normalise.begin(), [&](double fit) -> double {
         return 1. / (fit - g_optimal_fitness) * (fit - g_optimal_fitness);
      });
      fitness_to_normalise_sum = std::accumulate(fitness_to_normalise.begin(), fitness_to_normalise.end(), decltype(fitness_to_normalise)::value_type(0));
      std::transform(fitness_to_normalise.begin(), fitness_to_normalise.end(), population.begin(), normalised_fits.begin(), [&fitness_to_normalise_sum](double fit, Variables vars) {
         fit /= fitness_to_normalise_sum;
         return std::make_pair(fit, vars);
      });
      // Uncomment to arrenge roulette from lowest to highest fitnesses:
      // std::sort(normalised_fits.begin(), normalised_fits.end(), [](auto &left, auto &right) {
      //    return left.first < right.first;
      // });

      // selection
      while (selected.size() < g_population_size * 2) {
         double value = rand.next_double(1.0);
         for (auto fnv : normalised_fits) {
            value -= fnv.first;
            if (value <= 0) {
               // Draw without return
               // if(selected_map.find(fnv.first) == selected_map.end())
               //    continue;
               selected.push_back(fnv.second);
               break;
            }
         }
      }

      // crossover
      std::size_t crossovers_count{rand.next_int(0, g_population_size)};
      std::set<std::size_t> parents{};
      std::size_t omega{};// overrelaxation coefficient
      while (crossovers.size() < crossovers_count) {
         std::size_t fatherId{rand.next_int(0, g_population_size * 2)};
         std::size_t motherId{rand.next_int(0, g_population_size * 2)};
         if (crossovers.size() != 0)
            if (parents.find(fatherId) == parents.end() && parents.find(motherId) == parents.end()) {
               parents.insert(fatherId);
               parents.insert(motherId);
            } else
               continue;
         fmt::print("cross ");
         omega = rand.next_double(1.0);
         double alpha_initial, beta_initial, evaporation_rate_initial, alpha_final, beta_final, evaporation_rate_final;
         if (rand.next_double(1.0) < g_cross_chance) {
            alpha_initial = (1 - omega) * selected[motherId].alpha_initial + omega * selected[fatherId].alpha_initial;
         } else
            alpha_initial = selected[motherId].alpha_initial;
         if (rand.next_double(1.0) < g_cross_chance) {
            beta_initial = (1 - omega) * selected[motherId].beta_initial + omega * selected[fatherId].beta_initial;
         } else
            beta_initial = selected[motherId].beta_initial;
         if (rand.next_double(1.0) < g_cross_chance) {
            evaporation_rate_initial = (1 - omega) * selected[motherId].evaporation_rate_initial + omega * selected[fatherId].evaporation_rate_initial;
         } else
            evaporation_rate_initial = selected[motherId].evaporation_rate_initial;
         if (rand.next_double(1.0) < g_cross_chance) {
            alpha_final = (1 - omega) * selected[motherId].alpha_final + omega * selected[fatherId].alpha_final;
         } else
            alpha_final = selected[motherId].alpha_final;
         if (rand.next_double(1.0) < g_cross_chance) {
            beta_final = (1 - omega) * selected[motherId].beta_final + omega * selected[fatherId].beta_final;
         } else
            beta_final = selected[motherId].beta_final;
         if (rand.next_double(1.0) < g_cross_chance) {
            evaporation_rate_final = (1 - omega) * selected[motherId].evaporation_rate_final + omega * selected[fatherId].evaporation_rate_final;
         } else
            evaporation_rate_final = selected[motherId].evaporation_rate_final;

         // fmt::print("\ncrossover result:\n");
         // fmt::print("  alpha_initial: {}\n", alpha_initial);
         // fmt::print("  beta_initial: {}\n", beta_initial);
         // fmt::print("  evaporation_initial: {}\n", evaporation_rate_initial);
         // fmt::print("  alpha_final: {}\n", alpha_final);
         // fmt::print("  beta_final: {}\n", beta_final);
         // fmt::print("  evaporation_final: {}\n", evaporation_rate_final);
         crossovers.emplace_back(Variables{
                 std::clamp(alpha_initial, constraint.alpha_initial.min, constraint.alpha_initial.max),
                 std::clamp(beta_initial, constraint.beta_initial.min, constraint.beta_initial.max),
                 std::clamp(evaporation_rate_initial, constraint.evaporation_rate_initial.min, constraint.evaporation_rate_initial.max),
                 std::clamp(alpha_final, constraint.alpha_final.min, constraint.alpha_final.max),
                 std::clamp(beta_final, constraint.beta_final.min, constraint.beta_final.max),
                 std::clamp(evaporation_rate_final, constraint.evaporation_rate_final.min, constraint.evaporation_rate_final.max)});
      }
      // fill population with uncrossed parents
      while (crossovers.size() < g_population_size) {
         std::size_t singleId{rand.next_int(0, g_population_size * 2)};
         if (parents.find(singleId) == parents.end()) {
            parents.insert(singleId);
         } else
            continue;
         fmt::print("single ");
         parents.insert(singleId);

         // fmt::print("\nsingle parent result:\n");
         // fmt::print("  alpha_initial: {}\n", selected[singleId].alpha_initial);
         // fmt::print("  beta_initial: {}\n", selected[singleId].beta_initial);
         // fmt::print("  evaporation_initial: {}\n", selected[singleId].evaporation_rate_initial);
         // fmt::print("  alpha_final: {}\n", selected[singleId].alpha_final);
         // fmt::print("  beta_final: {}\n", selected[singleId].beta_final);
         // fmt::print("  evaporation_final: {}\n", selected[singleId].evaporation_rate_final);
         crossovers.emplace_back(Variables{
                 std::clamp(selected[singleId].alpha_initial, constraint.alpha_initial.min, constraint.alpha_initial.max),
                 std::clamp(selected[singleId].beta_initial, constraint.beta_initial.min, constraint.beta_initial.max),
                 std::clamp(selected[singleId].evaporation_rate_initial, constraint.evaporation_rate_initial.min, constraint.evaporation_rate_initial.max),
                 std::clamp(selected[singleId].alpha_final, constraint.alpha_final.min, constraint.alpha_final.max),
                 std::clamp(selected[singleId].beta_final, constraint.beta_final.min, constraint.beta_final.max),
                 std::clamp(selected[singleId].evaporation_rate_final, constraint.evaporation_rate_final.min, constraint.evaporation_rate_final.max)});
      }

      // mutation
      std::transform(crossovers.begin(), crossovers.end(), population.begin(), [&rand, &constraint](Variables &crossover) {
         double alpha_initial = 0, beta_initial = 0, evaporation_rate_initial = 0, alpha_final = 0, beta_final = 0, evaporation_rate_final = 0;
         if (rand.next_double(1.0) < g_mutation_chance) {

            // fmt::print("\nbefore mutation result:\n");
            // fmt::print("  alpha_initial: {}\n", crossover.alpha_initial);
            // fmt::print("  beta_initial: {}\n", crossover.beta_initial);
            // fmt::print("  evaporation_initial: {}\n", crossover.evaporation_rate_initial);
            // fmt::print("  alpha_final: {}\n", crossover.alpha_final);
            // fmt::print("  beta_final: {}\n", crossover.beta_final);
            // fmt::print("  evaporation_final: {}\n", crossover.evaporation_rate_final);
            alpha_initial = crossover.alpha_initial * (1 + 2 * rand.next_double(g_mutation_rate) - g_mutation_rate);
         } else
            alpha_initial = crossover.alpha_initial;
         if (rand.next_double(1.0) < g_mutation_chance) {
            beta_initial = crossover.beta_initial * (1 + 2 * rand.next_double(g_mutation_rate) - g_mutation_rate);
         } else
            beta_initial = crossover.beta_initial;
         if (rand.next_double(1.0) < g_mutation_chance) {
            evaporation_rate_initial = crossover.evaporation_rate_initial * (1 + 2 * rand.next_double(g_mutation_rate) - g_mutation_rate);
         } else
            beta_initial = crossover.beta_initial;
         if (rand.next_double(1.0) < g_mutation_chance) {
            alpha_final = crossover.alpha_final * (1 + 2 * rand.next_double(g_mutation_rate) - g_mutation_rate);
         } else
            beta_initial = crossover.beta_initial;
         if (rand.next_double(1.0) < g_mutation_chance) {
            beta_final = crossover.beta_final * (1 + 2 * rand.next_double(g_mutation_rate) - g_mutation_rate);
         } else
            beta_initial = crossover.beta_initial;
         if (rand.next_double(1.0) < g_mutation_chance) {
            evaporation_rate_final = crossover.evaporation_rate_final * (1 + 2 * rand.next_double(g_mutation_rate) - g_mutation_rate);
         } else
            beta_initial = crossover.beta_initial;

         // fmt::print("\nmutation result:\n");
         // fmt::print("  alpha_initial: {}\n", alpha_initial);
         // fmt::print("  beta_initial: {}\n", beta_initial);
         // fmt::print("  evaporation_initial: {}\n", evaporation_rate_initial);
         // fmt::print("  alpha_final: {}\n", alpha_final);
         // fmt::print("  beta_final: {}\n", beta_final);
         // fmt::print("  evaporation_final: {}\n", evaporation_rate_final);
         return Variables{
                 std::clamp(alpha_initial, constraint.alpha_initial.min, constraint.alpha_initial.max),
                 std::clamp(beta_initial, constraint.beta_initial.min, constraint.beta_initial.max),
                 std::clamp(evaporation_rate_initial, constraint.evaporation_rate_initial.min, constraint.evaporation_rate_initial.max),
                 std::clamp(alpha_final, constraint.alpha_final.min, constraint.alpha_final.max),
                 std::clamp(beta_final, constraint.beta_final.min, constraint.beta_final.max),
                 std::clamp(evaporation_rate_final, constraint.evaporation_rate_final.min, constraint.evaporation_rate_final.max)};
      });

      fmt::print("\nEWALUATING {}/{} GENERATION\n", i + 1, g_generations_count);
      std::transform(population.begin(), population.end(), fitness.begin(), objective_function);
      auto it_optimum = std::min(fitness.begin(), fitness.end());
      fmt::print("EWALUATION FINISHED\n\n");

      for (std::size_t p = 0; p < g_population_size; ++p)
         if (fitness[p] < *it_optimum) {
            it_optimum_population = generations.begin() + i;
            it_optimum = fitness.begin() + p;
         }
   }

   return generations[it_optimum_population - generations.begin()][it_optimum - fitness.begin()];
}

}// namespace msi::evolution