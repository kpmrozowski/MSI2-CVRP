#include <MSI/CVRP/CVRP.h>
#include <MSI/Evolution/Evolution.h>
#include <boost/program_options.hpp>
#include <fmt/core.h>

namespace opts = boost::program_options;
constexpr auto g_version = "0.0.1";

msi::evolution::ObjectiveFunction make_objective_function(msi::util::IRandomGenerator &rand, msi::cvrp::Params &params, const std::string &coords_file, const std::string &demands_file) {
   return [&params,
           &rand,
           graph = msi::cvrp::graph_from_file(params, coords_file, demands_file)](const msi::evolution::Variables &vars) -> double {
      params.alpha = vars.alpha;
      params.beta = vars.beta;
      params.evaporation_rate = vars.evaporation_rate;
      auto graph_copy = msi::cvrp::Graph(graph);
      auto result =  msi::cvrp::train(rand, params, graph_copy);
      fmt::print("T {}\n", result);
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
   msi::evolution::Constraint constraint{
           {1.0, 2.0},
           {2.0, 5.0},
           {0.9, 0.99},
   };

   auto result = msi::evolution::FindOptimal(rand, make_objective_function(rand, params, input_files[0], input_files[1]), constraint);
   fmt::print("final result:\n");
   fmt::print("  alpha: {}\n", result.alpha);
   fmt::print("  beta: {}\n", result.beta);
   fmt::print("  evaporation: {}\n", result.evaporation_rate);
}