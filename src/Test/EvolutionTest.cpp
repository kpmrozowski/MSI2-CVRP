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

   auto result = msi::evolution::FindOptimal(r, objective_function, constraint);
   ASSERT_LE(result.alpha_initial, 0.2);
   ASSERT_LE(result.beta_initial, 0.3);
   ASSERT_GE(result.evaporation_rate_initial, 1.9);
   ASSERT_LE(result.alpha_final, 0.2);
   ASSERT_LE(result.beta_final, 0.3);
   ASSERT_GE(result.evaporation_rate_final, 1.9);
}
