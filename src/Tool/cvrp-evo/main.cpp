#include <MSI/CVRP/CVRP.h>
#include <MSI/Evolution/Evolution.h>
#include <MSI/Util/drawGL.h>
#include <boost/program_options.hpp>
#include <fmt/core.h>

constexpr auto g_population_size = 6;
constexpr auto g_generations_count = 2;
constexpr auto g_mutation_chance = 0.8;
constexpr auto g_cross_chance = 0.8;
constexpr auto g_mutation_rate = 0.1;// max change: 10%
constexpr auto g_optimal_fitness = 521.;

namespace opts = boost::program_options;
constexpr auto g_version = "0.0.1";

msi::evolution::ObjectiveFunction make_objective_function(std::vector<msi::cvrp::Tour> &tours, msi::util::IRandomGenerator &rand, msi::cvrp::Params &params, const std::string &coords_file, const std::string &demands_file) {

   return [&tours,
           &params,
           &rand,
           graph = msi::cvrp::graph_from_file(params, coords_file, demands_file)](const msi::evolution::Variables &vars) -> double {
      params.alpha_initial = vars.alpha_initial;
      params.beta_initial = vars.beta_initial;
      params.evaporation_rate_initial = vars.evaporation_rate_initial;
      params.alpha_final = vars.alpha_final;
      params.beta_final = vars.beta_final;
      params.evaporation_rate_final = vars.evaporation_rate_final;
      auto graph_copy = msi::cvrp::Graph(graph);
      auto result = msi::cvrp::train(tours, rand, params, graph_copy);
      fmt::print("\nfinal result:\n");
      fmt::print("  alpha_initial: {}\n", params.alpha_initial);
      fmt::print("  beta_initial: {}\n", params.beta_initial);
      fmt::print("  evaporation_initial: {}\n", params.evaporation_rate_initial);
      fmt::print("  alpha_final: {}\n", params.alpha_final);
      fmt::print("  beta_final: {}\n", params.beta_final);
      fmt::print("  evaporation_final: {}\n", params.evaporation_rate_final);
      fmt::print("  Fitness: {}\n", result);
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

   srand(time(0));
   msi::util::Random rand;
   msi::cvrp::Params params;
   msi::evolution::Constraint constraint{
           //   {0.8, 2.0},
           //   {1.6, 5.0},
           //   {0.85, 0.99},
           //   {0.8, 2.0},
           //   {1.6, 5.0},
           //   {0.85, 0.99},
           {0.1, 2.0},
           {3.0, 9.0},
           {0.85, 0.999},
           {1.0, 8.0},
           {0.01, 5.0},
           {0.5, 0.9},
   };
   std::vector<msi::cvrp::Tour> tours;

   msi::evolution::Params evo_params{
           .population_size = g_population_size,
           .generations_count = g_generations_count,
           .mutation_chance = g_mutation_chance,
           .cross_chance = g_cross_chance,
           .mutation_rate = g_mutation_rate,
           .optimal_fitness = g_optimal_fitness,
   };

   auto objective_func = make_objective_function(tours, rand, params, input_files[0], input_files[1]);
   auto result = msi::evolution::FindOptimal(rand, objective_func, evo_params, constraint);

   fmt::print("final result:\n");
   fmt::print("  alpha_initial: {}\n", result.second.alpha_initial);
   fmt::print("  beta_initial: {}\n", result.second.beta_initial);
   fmt::print("  evaporation_initial: {}\n", result.second.evaporation_rate_initial);
   fmt::print("  alpha_final: {}\n", result.second.alpha_final);
   fmt::print("  beta_final: {}\n", result.second.beta_final);
   fmt::print("  evaporation_final: {}\n", result.second.evaporation_rate_final);
   fmt::print("  Fitness: {}\n", result.first);

   msi::util::GraphElements graphElements{};
   // msi::cvrp::Tour tour = tours[0];
   fmt::print("tours.size={}\n", tours.size());
   for (auto tour : tours)
      fmt::print("{}\n", tour.min_distance());
   graphElements.translate_vert_into_edges(tours[0]);

   msi::util::Opengl opengl;
   opengl.draw(graphElements);
}