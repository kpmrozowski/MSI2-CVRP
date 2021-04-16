#include <MSI/CVRP/CVRP.h>
#include <MSI/CVRP/Params.h>
#include <MSI/Util/IRandomGenerator.h>
#include <MSI/Util/ReadGraph.h>
#include <MSI/Util/drawGL.h>

namespace msi::cvrp {

void CVRP::start_cvrp() noexcept {
   // srand(102);
   msi::util::Random r;
   msi::util::Reader re;
   // re.display_file();
   std::size_t rows = 51;
   std::size_t cols = 3;
   Table En51k5_VERT_COORD, En51k5_DEMANDS;
   En51k5_VERT_COORD = re.read_file("./src/CVRP/E-n51-k5_VERT_COORD.txt", rows, cols);
   cols = 2;
   En51k5_DEMANDS = re.read_file("./src/CVRP/E-n51-k5_DEMANDS.txt", rows, cols);
   Params p;
   Graph graph(rows);
   graph.import_vertices(En51k5_VERT_COORD, En51k5_DEMANDS);
   graph.compute_distances();
   for (std::size_t i = 0; i < rows; i++)
      for (std::size_t j = i + 1; j < rows; j++)
         graph.connect(i, j, Edge(1.0, Position(En51k5_VERT_COORD[1][i], En51k5_VERT_COORD[2][i], En51k5_VERT_COORD[1][j], En51k5_VERT_COORD[2][j])));

   std::vector<double> distances(p.iterations);
   Tour tour(graph, p, r);
   for (std::size_t iter_n = 0; iter_n < p.iterations; ++iter_n) {
      tour.m_current_iter = iter_n;
      tour.run(iter_n);
      distances[iter_n] = tour.shortest_distance().first;
   }
   auto it_min_dist = min(distances.begin(), distances.end());
   fmt::print("\nShortest_distance = {}", *it_min_dist);

   // graph.print();

   msi::util::GraphElements graphElements{};
   graphElements.translate_vert_into_edges(tour);

   msi::util::Opengl opengl;
   opengl.draw(graphElements);
}

static std::pair<double, double> regression(std::vector<double> &values) {
   auto x_mean = (values.size()/2-1.0) / 2.0;
   std::vector<double>::iterator it_half = (values.begin() + values.size()/2);
   auto y_mean = std::accumulate(it_half, values.end(), 0.0) / values.size()/2;

   auto denom = 0.0;
   for (std::size_t i = values.size()/2; i < values.size(); ++i) {
      denom += std::pow(static_cast<double>(i) - x_mean, 2.0);
   }

   auto beta = std::accumulate(it_half, values.end(), 0.0, [x_mean, y_mean, x = 0.0](double acc, double y) mutable {
      return acc + ((x++) - x_mean) * (y - y_mean);
   }) / denom;
   return std::make_pair(y_mean - beta * x_mean, beta);
}

double train(std::vector<std::unique_ptr<msi::cvrp::Tour>> &tours, util::IRandomGenerator &rand, const Params &params, msi::evolution::Params &evo_params, const Graph &graph) {
   std::vector<double> distances(params.iterations);
   msi::cvrp::Tour tour(graph, params, rand);
   for (std::size_t i = 0; i < params.iterations; ++i) {
      tour.m_current_iter = i;
      tour.run(i);
      distances[i] = tour.shortest_distance().first;
   }
   tours.push_back(std::make_unique<msi::cvrp::Tour>(tour));

   auto reg = regression(distances);
   return tour.min_distance() - evo_params.optimal_fitness/2 - 100 - 5 * params.beta[0] + params.iterations * reg.second;
}

Graph graph_from_file(const std::string &fn_coords, const std::string &fn_demands) {
   msi::util::Reader re;
   std::size_t rows = 51;
   std::size_t cols = 3;

   auto coords = re.read_file(fn_coords, rows, cols);
   cols = 2;
   auto demands = re.read_file(fn_demands, rows, cols);

   Graph graph(rows);
   graph.import_vertices(coords, demands);
   graph.compute_distances();

   for (std::size_t i = 0; i < rows; i++) {
      for (std::size_t j = i + 1; j < rows; j++) {
         graph.connect(i, j, Edge(1.0, Position(coords[1][i], coords[2][i], coords[1][j], coords[2][j])));
      }
   }

   return graph;
}

}// namespace msi::cvrp