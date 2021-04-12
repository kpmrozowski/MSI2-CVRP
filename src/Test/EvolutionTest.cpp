#include <gtest/gtest.h>
#include <MSI/Evolution/Evolution.h>
#include <MSI/Util/IRandomGenerator.h>

TEST(Evolution, Simple) {
   auto objective_function = [](const msi::evolution::Variables &vars) -> double {
      return vars.alpha + vars.beta - vars.evaporation_rate;
   };

   msi::util::Random r;

   msi::evolution::Constraint constraint{
           {0.1, 2.0},
           {0.2, 2.0},
           {0.3, 2.0},
   };

   auto result = msi::evolution::FindOptimal(r, objective_function, constraint);
   ASSERT_LE(result.alpha, 0.2);
   ASSERT_LE(result.beta, 0.3);
   ASSERT_GE(result.evaporation_rate, 1.9);
}
