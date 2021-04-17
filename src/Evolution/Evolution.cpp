#include <MSI/Evolution/Evolution.h>
#include <fmt/core.h>
#include <fmt/os.h>

namespace msi::evolution {

std::pair<double, Variables> FindOptimal(std::vector<std::unique_ptr<msi::cvrp::Tour>> &tours, util::IRandomGenerator &rand, const ObjectiveFunction &objective_function, Params evo_params, Constraint constraint) {
   std::size_t ant_nr{0};
   std::vector<Variables> population(evo_params.population_size);
   std::vector<Variables> optimal_over_generations(evo_params.generations_count);
   std::vector<double> optimal_fitness(evo_params.generations_count);
   msi::cvrp::Params params;

   std::generate(population.begin(), population.end(), [&params, &rand, &constraint, &evo_params]() {
      std::vector<double> alpha(params.polynomial_degree + 1, 0);
      std::vector<double> beta(params.polynomial_degree + 1, 0);
      std::vector<double> evaporation_rate(params.polynomial_degree + 1, 0);
      // Uncomment to start from boundary limits:
      // for (std::size_t i = 0; i <= params.polynomial_degree; i++) {
      //    alpha[i] = rand.next_double(constraint.alpha[i].min, constraint.alpha[i].max);
      //    beta[i] = rand.next_double(constraint.beta[i].min, constraint.beta[i].max);
      //    evaporation_rate[i] = rand.next_double(constraint.evaporation_rate[i].min, constraint.evaporation_rate[i].max);
      // }
      // Uncomment to start from custom limits:
      double low = 1 - evo_params.mutation_rate_initial;
      double high = 1 + evo_params.mutation_rate_initial;

      alpha = {0.75 * rand.next_double(low, high),
               1.1 * rand.next_double(low, high),
               0.25 * rand.next_double(low, high)};
      beta = {3.2 * rand.next_double(low, high),
              3.2 * rand.next_double(low, high),
              0.3 * rand.next_double(low, high)};
      evaporation_rate = {0.9 * rand.next_double(low, high),
                          0.9 * rand.next_double(low, high),
                          0.3 * rand.next_double(low, high)};
      return Variables{
              alpha,
              beta,
              evaporation_rate,
      };
   });

   std::vector<std::pair<double, Variables>> fits_vars(evo_params.population_size);
   std::vector<std::vector<std::pair<double, Variables>>> generations(evo_params.generations_count);
   std::vector<double> fitness(evo_params.population_size);
   std::vector<double> fitness_to_normalise(evo_params.population_size);
   double fitness_to_normalise_sum{};
   std::vector<std::pair<double, Variables>> normalised_fits(evo_params.population_size);
   fmt::print("\nEVALUATING {} ANTS\n", evo_params.population_size);


   std::vector<std::future<double>> future_fitness(evo_params.population_size);
   std::transform(population.begin(), population.end(), future_fitness.begin(), [&objective_function](const Variables &vars) {
      return std::async(std::launch::async, objective_function, vars);
   });
   std::transform(future_fitness.begin(), future_fitness.end(), fitness.begin(), [&ant_nr](std::future<double> &f) {
      ant_nr++;
      fmt::print("ant{}, ", ant_nr);
      return f.get();
   });

   auto it_optimum = std::min(fitness.begin(), fitness.end());
   auto it_optimum_population = generations.begin();
   std::transform(fitness.begin(), fitness.end(), population.begin(), fits_vars.begin(), [](double fit, Variables vars) {
      return std::make_pair(fit, vars);
   });


   for (std::size_t i = 0; i < evo_params.generations_count; ++i) {
      generations[i] = fits_vars;
      std::vector<Variables> selected;
      std::vector<Variables> crossovers;
      std::vector<Variables> mutants(evo_params.population_size);
      std::transform(fitness.begin(), fitness.end(), fitness_to_normalise.begin(), [&](double fit) -> double {
         if (fit <= 0) fit = 2.;
         return 1. / pow(fit, 4.);
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
      while (selected.size() < evo_params.population_size * 2) {
         double value = rand.next_double(1.0);
         for (auto fnv : normalised_fits) {
            value -= fnv.first;
            if (value <= 0) {
               selected.push_back(fnv.second);
               break;
            }
         }
      }

      // crossover
      auto crossovers_count = static_cast<std::size_t>(rand.next_int(evo_params.population_size / 2, evo_params.population_size));
      fmt::print("\n{} crosses, {} singles", crossovers_count, evo_params.population_size - crossovers_count);
      std::set<std::size_t> parents{};
      std::size_t omega{};// overrelaxation coefficient
      while (crossovers.size() < crossovers_count) {
         auto fatherId = static_cast<std::size_t>(rand.next_int(0, evo_params.population_size * 2));
         auto motherId = static_cast<std::size_t>(rand.next_int(0, evo_params.population_size * 2));
         if (!crossovers.empty()) {
            if (parents.find(fatherId) != parents.end() || parents.find(motherId) != parents.end()) {
               continue;
            }
            parents.insert(fatherId);
            parents.insert(motherId);
         }
         omega = rand.next_double(1.0);
         std::vector<double> alpha(params.polynomial_degree + 1, 0);
         std::vector<double> beta(params.polynomial_degree + 1, 0);
         std::vector<double> evaporation_rate(params.polynomial_degree + 1, 0);
         for (std::size_t i = 0; i <= params.polynomial_degree; i++) {
            if (rand.next_double(1.0) < evo_params.cross_chance) {
               alpha[i] = (1.0 - omega) * selected[motherId].alpha[i] + omega * selected[fatherId].alpha[i];
            } else
               alpha[i] = selected[motherId].alpha[i];
            if (rand.next_double(1.0) < evo_params.cross_chance) {
               beta[i] = (1.0 - omega) * selected[motherId].beta[i] + omega * selected[fatherId].beta[i];
            } else
               beta[i] = selected[motherId].beta[i];
            if (rand.next_double(1.0) < evo_params.cross_chance) {
               evaporation_rate[i] = (1.0 - omega) * selected[motherId].evaporation_rate[i] + omega * selected[fatherId].evaporation_rate[i];
            } else
               evaporation_rate[i] = selected[motherId].evaporation_rate[i];
         }
         for (std::size_t i = 0; i <= params.polynomial_degree; i++) {
            alpha[i] = std::clamp(alpha[i], constraint.alpha[i].min, constraint.alpha[i].max);
            beta[i] = std::clamp(beta[i], constraint.beta[i].min, constraint.beta[i].max);
            evaporation_rate[i] = std::clamp(evaporation_rate[i], constraint.evaporation_rate[i].min, constraint.evaporation_rate[i].max);
         }
         crossovers.emplace_back(Variables{alpha, beta, evaporation_rate});
      }
      // fill population with uncrossed parents
      while (crossovers.size() < evo_params.population_size) {
         auto singleId = static_cast<std::size_t>(rand.next_int(0, evo_params.population_size * 2));
         if (parents.find(singleId) == parents.end()) {
            parents.insert(singleId);
         } else
            continue;
         parents.insert(singleId);
         std::vector<double> alpha(params.polynomial_degree + 1, 0);
         std::vector<double> beta(params.polynomial_degree + 1, 0);
         std::vector<double> evaporation_rate(params.polynomial_degree + 1, 0);
         for (std::size_t i = 0; i <= params.polynomial_degree; i++) {
            alpha[i] = std::clamp(selected[singleId].alpha[i], constraint.alpha[i].min, constraint.alpha[i].max);
            beta[i] = std::clamp(selected[singleId].beta[i], constraint.beta[i].min, constraint.beta[i].max);
            evaporation_rate[i] = std::clamp(selected[singleId].evaporation_rate[i], constraint.evaporation_rate[i].min, constraint.evaporation_rate[i].max);
         }
         crossovers.emplace_back(Variables{alpha, beta, evaporation_rate});
      }

      // mutation
      std::transform(crossovers.begin(), crossovers.end(), population.begin(), [&i, &params, &rand, &constraint, &evo_params](Variables &crossover) {
         double mutation_rate = evo_params.mutation_rate_initial + i / static_cast<double>(evo_params.generations_count) * (evo_params.mutation_rate_final - evo_params.mutation_rate_initial);
         std::vector<double> alpha(params.polynomial_degree + 1, 0);
         std::vector<double> beta(params.polynomial_degree + 1, 0);
         std::vector<double> evaporation_rate(params.polynomial_degree + 1, 0);
         for (std::size_t i = 0; i <= params.polynomial_degree; i++) {
            if (rand.next_double(1.0) < evo_params.mutation_chance) {
               alpha[i] = crossover.alpha[i] * (1 + 2 * rand.next_double(mutation_rate) - mutation_rate);
            } else
               alpha[i] = crossover.alpha[i];
            if (rand.next_double(1.0) < evo_params.mutation_chance) {
               beta[i] = crossover.beta[i] * (1 + 2 * rand.next_double(mutation_rate) - mutation_rate);
            } else
               beta[i] = crossover.beta[i];
            if (rand.next_double(1.0) < evo_params.mutation_chance) {
               evaporation_rate[i] = crossover.evaporation_rate[i] * (1 + 2 * rand.next_double(mutation_rate) - mutation_rate);
            } else
               evaporation_rate[i] = crossover.beta[i];
         }
         for (std::size_t i = 0; i <= params.polynomial_degree; i++) {
            alpha[i] = std::clamp(alpha[i], constraint.alpha[i].min, constraint.alpha[i].max);
            beta[i] = std::clamp(beta[i], constraint.beta[i].min, constraint.beta[i].max);
            evaporation_rate[i] = std::clamp(evaporation_rate[i], constraint.evaporation_rate[i].min, constraint.evaporation_rate[i].max);
         }

         return Variables{alpha, beta, evaporation_rate};
      });

      fmt::print("\n\nEWALUATING {}/{} GENERATION\n", i, evo_params.generations_count);
      std::transform(population.begin(), population.end(), future_fitness.begin(), [&objective_function](const Variables &vars) {
         return std::async(std::launch::async, objective_function, vars);
      });
      std::transform(future_fitness.begin(), future_fitness.end(), fitness.begin(), [&ant_nr](std::future<double> &f) {
         ant_nr++;
         fmt::print("ant{}, ", ant_nr);
         return f.get();
      });
      std::transform(fitness.begin(), fitness.end(), population.begin(), fits_vars.begin(), [](double fit, Variables vars) {
         return std::make_pair(fit, vars);
      });
      auto it_optimum = std::min(fitness.begin(), fitness.end());
      optimal_over_generations[i] = population[it_optimum - fitness.begin()];
      optimal_fitness[i] = *it_optimum;
      fmt::print("\nEVALUATION FINISHED\n\n");

      for (std::size_t p = 0; p < evo_params.population_size; ++p)
         if (fitness[p] < *it_optimum) {
            it_optimum_population = generations.begin() + i;
            it_optimum = fitness.begin() + p;
         }
   }


   auto log = fmt::output_file("evo_params.csv");
   log.print("iteration,fitness,");
   for (std::size_t i = 0; i < 3; i++)
      for (std::size_t j = 0; j <= params.polynomial_degree; j++) {
         if (i == 0) log.print("alpha{},", j);
         if (i == 1) log.print("beta{},", j);
         if (i == 2)
            if (j != params.polynomial_degree)
               log.print("evaporation_rate{},", j);
            else
               log.print("evaporation_rate{}", j);
      }
   auto iter = 0;
   for (const auto &opt : optimal_over_generations) {
      log.print("\n{},{},", iter + 1, optimal_fitness[iter]);
      for (std::size_t i = 0; i < 3; i++)
         for (std::size_t j = 0; j <= params.polynomial_degree; j++) {
            if (i == 0) log.print("{},", opt.alpha[j]);
            if (i == 1) log.print("{},", opt.beta[j]);
            if (i == 2) {
               if (j != params.polynomial_degree)
                  log.print("{},", opt.evaporation_rate[j]);
               else
                  log.print("{}", opt.evaporation_rate[j]);
            }
         }
      ++iter;
   }
   log.close();

   auto log_fit = fmt::output_file("fitness_distances_params.csv");
   log_fit.print("n,generation,specimen,fitness,distance,");
   for (std::size_t i = 0; i < 3; i++)
      for (std::size_t j = 0; j <= params.polynomial_degree; j++) {
         if (i == 0) log_fit.print("alpha{},", j);
         if (i == 1) log_fit.print("beta{},", j);
         if (i == 2) {
            if (j != params.polynomial_degree)
               log_fit.print("evaporation_rate{},", j);
            else
               log_fit.print("evaporation_rate{}", j);
         }
      }
   iter = 0;
   for (std::size_t gen = 0; gen < evo_params.generations_count; ++gen) {
      for (std::size_t pop = 0; pop < evo_params.population_size; ++pop) {
         log_fit.print("\n{},{},{},{},{},", iter + 1, gen, pop, generations[gen][pop].first, tours[evo_params.population_size * (gen + 1) + pop]->min_distance());
         for (std::size_t i = 0; i < 3; i++)
            for (std::size_t j = 0; j <= params.polynomial_degree; j++) {
               if (i == 0) log_fit.print("{},", generations[gen][pop].second.alpha[j]);
               if (i == 1) log_fit.print("{},", generations[gen][pop].second.beta[j]);
               if (i == 2) {
                  if (j != params.polynomial_degree)
                     log_fit.print("{},", generations[gen][pop].second.evaporation_rate[j]);
                  else
                     log_fit.print("{}", generations[gen][pop].second.evaporation_rate[j]);
               }
            }
         ++iter;

         ++iter;
      }
   }

   return generations[it_optimum_population - generations.begin()][it_optimum - fitness.begin()];
}// namespace msi::evolution

}// namespace msi::evolution