#include <MSI/Evolution/Evolution.h>
#include <MSI/Util/IRandomGenerator.h>
#include <gtest/gtest.h>

TEST(Evolution, Simple) {
   auto objective_function = [](const msi::evolution::Variables &vars) -> double {
           return 0; 
//       return vars.alpha_final + (12 - 0) * (vars.alpha_final - vars.alpha_initial) / (12 - 0) + vars.beta_initial + (12 - 0) * (vars.beta_final - vars.beta_initial) / (12 - 0) - vars.evaporation_rate_initial + (12 - 0) * (vars.evaporation_rate_final - vars.evaporation_rate_initial) / (12 - 0);

   };

   msi::util::Random r;

   msi::evolution::Constraint constraint{
           std::vector<msi::evolution::Range>({{.1, .5}, {.00001, .5}, {.00001, .5}, {0.00001, 0.1}}),
           std::vector<msi::evolution::Range>({{4., 10.}, {.00001, 10.}, {.00001, 10.}, {0.00001, 0.001}}),
           std::vector<msi::evolution::Range>({{.01, .6}, {.00001, .6}, {.00001, .6}, {0.00001, 0.01}}),
   };

   msi::evolution::Params params{
           .population_size = 24,
           .generations_count = 10,
           .mutation_chance = 0.8,
           .cross_chance = 0.8,
           .mutation_rate_initial = 0.1,
           .mutation_rate_final = 0.1,
           .optimal_fitness = 521,
   };
        
   std::vector<std::unique_ptr<msi::cvrp::Tour>> tours;
   auto result = msi::evolution::FindOptimal(tours, r, objective_function, params, constraint);

//    ASSERT_LE(result.second.alpha_initial, 0.2);
//    ASSERT_LE(result.second.beta_initial, 0.3);
//    ASSERT_GE(result.second.evaporation_rate_initial, 1.9);
//    ASSERT_LE(result.second.alpha_final, 0.2);
//    ASSERT_LE(result.second.beta_final, 0.3);
//    ASSERT_GE(result.second.evaporation_rate_final, 1.9);
}
