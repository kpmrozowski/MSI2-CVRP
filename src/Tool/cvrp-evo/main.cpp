#include <MSI/CVRP/CVRP.h>
#include <MSI/Evolution/Evolution.h>
#include <MSI/Util/drawGL.h>
#include <boost/program_options.hpp>
#include <fmt/core.h>

constexpr auto g_population_size = 36;
constexpr auto g_generations_count = 100;
constexpr auto g_mutation_chance = 0.9;
constexpr auto g_cross_chance = .6;
constexpr auto g_mutation_rate = 0.1;
constexpr auto g_optimal_fitness = 521.;

namespace opts = boost::program_options;

msi::evolution::ObjectiveFunction make_objective_function(std::vector<std::unique_ptr<msi::cvrp::Tour>> &tours, msi::util::IRandomGenerator &rand, msi::cvrp::Params &params, msi::evolution::Params &evo_params, const std::string &coords_file, const std::string &demands_file) {
   return [&tours,
           &evo_params,
           &rand,
           graph = msi::cvrp::graph_from_file(coords_file, demands_file)](const msi::evolution::Variables &vars) -> double {
      msi::cvrp::Params params;
      params.alpha = vars.alpha;
      params.beta = vars.beta;
      params.evaporation_rate = vars.evaporation_rate;
      auto result = msi::cvrp::train(tours, rand, params, evo_params, graph);
      fmt::print("\nfinal result:");
      fmt::print("\n  alpha:\t\t");
      for (auto a : params.alpha) fmt::print("{:0.5f}, ", a);
      fmt::print("\n  beta:\t\t\t");
      for (auto b : params.beta) fmt::print("{:0.5f}, ", b);
      fmt::print("\n  evaporation_rate:\t");
      for (auto er : params.evaporation_rate) fmt::print("{:0.5f}, ", er);
      fmt::print("\n  Fitness: {}\n", result);
      fmt::print("  Distance: {}\n", (*(tours.end() - 1))->min_distance());
      return result;
   };
}

int main(int argc, char **argv) {
   opts::options_description desc("cvrp-evo");
   desc.add_options()("help", "print help information")("version,v", "print version")("input,i", opts::value<std::vector<std::string>>(), "specify input files");

   opts::positional_options_description desc_pos;
   desc_pos.add("input", -1);

   opts::variables_map values;
   opts::store(opts::command_line_parser(argc, argv).options(desc).positional(desc_pos).run(), values);

   auto input_value = values["input"];
   if (input_value.empty()) {
      return 1;
   }
   auto input_files = input_value.as<std::vector<std::string>>();

   msi::util::Random rand;
   msi::cvrp::Params params;
   // msi::evolution::Constraint constraint{
   //         std::vector<msi::evolution::Range>({{.05, 20.},
   //                                             {.2, 20.},
   //                                           //   {.1, 1.},
   //                                             {.05, 20.}}),
   //         std::vector<msi::evolution::Range>({{1., 20.},
   //                                             {.05, 10.},
   //                                           //   {.001, 10.},
   //                                             {.05, 5.}}),
   //         std::vector<msi::evolution::Range>({{.7, .999},
   //                                             {.4, .999},
   //                                           //   {.001, .99},
   //                                             {.01, .999}}),
   // };
   msi::evolution::Constraint constraint{
      std::vector<msi::evolution::Range>(3, {.05, 20.}),
      std::vector<msi::evolution::Range>(3, {.001, 20.}),
      std::vector<msi::evolution::Range>(3, {.001, .999}),
   };

   std::vector<std::unique_ptr<msi::cvrp::Tour>> tours;

   msi::evolution::Params evo_params{
           .population_size = g_population_size,
           .generations_count = g_generations_count,
           .mutation_chance = g_mutation_chance,
           .cross_chance = g_cross_chance,
           .mutation_rate = g_mutation_rate,
           .optimal_fitness = g_optimal_fitness,
   };

   auto objective_func = make_objective_function(tours, rand, params, evo_params, input_files[0], input_files[1]);
   auto result = msi::evolution::FindOptimal(tours, rand, objective_func, evo_params, constraint);

   fmt::print("\nfinal result:");
   fmt::print("\n  alpha:\t\t");
   for (auto a : result.second.alpha) fmt::print("{:0.5f}, ", a);
   fmt::print("\n  beta:\t\t\t");
   for (auto b : result.second.beta) fmt::print("{:0.5f}, ", b);
   fmt::print("\n  evaporation_rate:\t");
   for (auto er : result.second.evaporation_rate) fmt::print("{:0.5f}, ", er);
   fmt::print("\n  Fitness: {}\n", result.first);
   fmt::print("  Distance: {}\n", (*(tours.end() - 1))->min_distance());

   fmt::print("tours.size={}\n", tours.size());

   std::sort(tours.begin(), tours.end(), [](const std::unique_ptr<msi::cvrp::Tour> &l, const std::unique_ptr<msi::cvrp::Tour> &r) {
      return l->min_distance() < r->min_distance();
   });

   for (const auto &tour : tours)
      fmt::print("{}\n", tour->min_distance());
   fmt::print("\nBEST DIST: {}\n", tours[0]->min_distance());
   msi::util::GraphElements graphElements1{};
   graphElements1.translate_vert_into_edges(*(tours[0]));
   msi::util::Opengl opengl1;
   opengl1.draw(graphElements1);
}