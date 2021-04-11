#include <gtest/gtest.h>
#include <MSI/Evolution/Evolution.h>
#include <MSI/Util/IRandomGenerator.h>
#include <fmt/core.h>

TEST(Evolution, Simple) {
   auto objective_function = [](const msi::evolution::Variables &vars) -> double {
      return vars.alpha + vars.beta + vars.evaporation_rate;
   };

   msi::util::Random r;

   msi::evolution::Constraint constraint{
           {0.1, 2.0},
           {0.2, 2.0},
           {0.3, 2.0},
   };

   auto result = msi::evolution::FindOptimal(r, objective_function, constraint);
   ASSERT_EQ(result.alpha, 0.1);
   ASSERT_EQ(result.beta, 0.2);
   ASSERT_EQ(result.evaporation_rate, 0.3);
}
