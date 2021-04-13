#include <gtest/gtest.h>
#include <MSI/Evolution/Evolution.h>
#include <MSI/Util/IRandomGenerator.h>

TEST(Evolution, Simple) {
   auto objective_function = [](const msi::evolution::Variables &vars) -> double {
      return vars.alpha_final + (12 - 0) * (vars.alpha_final - vars.alpha_initial)/(12 - 0) + vars.beta_initial + (12 - 0) * (vars.beta_final - vars.beta_initial)/(12 - 0) - vars.evaporation_rate_initial + (12 - 0) * (vars.evaporation_rate_final - vars.evaporation_rate_initial)/(12 - 0);
   };

   msi::util::Random r;

   msi::evolution::Constraint constraint{
           {0.1, 2.0},
           {0.2, 2.0},
           {0.3, 2.0},
           {0.1, 2.0},
           {0.2, 2.0},
           {0.3, 2.0},
   };

   constexpr auto g_population_size = 24;
   constexpr auto g_generations_count = 10;
   constexpr auto g_mutation_chance = 0.8;
   constexpr auto g_cross_chance = 0.8;
   constexpr auto g_mutation_rate = 0.1;// max change: 10%
   constexpr auto g_optimal_fitness = 521.;

   auto result = msi::evolution::FindOptimal(g_population_size, g_generations_count, g_mutation_chance, g_cross_chance, g_mutation_rate, g_optimal_fitness, r, objective_function, constraint);

   ASSERT_LE(result.second.alpha_initial, 0.2);
   ASSERT_LE(result.second.beta_initial, 0.3);
   ASSERT_GE(result.second.evaporation_rate_initial, 1.9);
   ASSERT_LE(result.second.alpha_final, 0.2);
   ASSERT_LE(result.second.beta_final, 0.3);
   ASSERT_GE(result.second.evaporation_rate_final, 1.9);
}
