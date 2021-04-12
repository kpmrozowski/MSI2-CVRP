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
      params.alpha_initial = vars.alpha_initial;
      params.beta_initial = vars.beta_initial;
      params.evaporation_rate_initial = vars.evaporation_rate_initial;
      params.alpha_final = vars.alpha_final;
      params.beta_final = vars.beta_final;
      params.evaporation_rate_final = vars.evaporation_rate_final;
      auto graph_copy = msi::cvrp::Graph(graph);
      auto result =  msi::cvrp::train(rand, params, graph_copy);
      fmt::print("\nfinal result:\n");
      fmt::print("  alpha_initial: {}\n", params.alpha_initial);
      fmt::print("  beta_initial: {}\n", params.beta_initial);
      fmt::print("  evaporation_initial: {}\n", params.evaporation_rate_initial);
      fmt::print("  alpha_final: {}\n", params.alpha_final);
      fmt::print("  beta_final: {}\n", params.beta_final);
      fmt::print("  evaporation_final: {}\n", params.evaporation_rate_final);
      fmt::print("  Fitness: {}\n", result);
      //   final result:
      //   alpha_initial: 1.3928528832710014
      //   beta_initial: 4.944063091326039
      //   evaporation_initial: 0.9310447281317501
      //   alpha_final: 0
      //   beta_final: 0
      //   evaporation_final: 0
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
           {0.8, 2.0},
           {1.6, 5.0},
           {0.85, 0.99},
           {0.8, 2.0},
           {1.6, 5.0},
           {0.85, 0.99},
   };

   auto result = msi::evolution::FindOptimal(rand, make_objective_function(rand, params, input_files[0], input_files[1]), constraint);
   fmt::print("final result:\n");
   fmt::print("  alpha_initial: {}\n", result.alpha_initial);
   fmt::print("  beta_initial: {}\n", result.beta_initial);
   fmt::print("  evaporation_initial: {}\n", result.evaporation_rate_initial);
   fmt::print("  alpha_final: {}\n", result.alpha_final);
   fmt::print("  beta_final: {}\n", result.beta_final);
   fmt::print("  evaporation_final: {}\n", result.evaporation_rate_final);
}