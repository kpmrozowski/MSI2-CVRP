#include <MSI/CVRP/CVRP.h>
#include <MSI/CVRP/Params.h>
#include <MSI/Util/IRandomGenerator.h>
#include <MSI/Util/ReadGraph.h>
#include <MSI/Util/drawGL.h>

namespace msi::cvrp {

void CVRP::start_cvrp() noexcept {
   srand(time(0));
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
   Graph graph(p, rows);
   graph.import_vertices(En51k5_VERT_COORD, En51k5_DEMANDS);
   graph.compute_distances();
   for (std::size_t i = 0; i < rows; i++)
      for (std::size_t j = i + 1; j < rows; j++)
         graph.connect(i, j, Edge(1.0, Position(En51k5_VERT_COORD[1][i], En51k5_VERT_COORD[2][i], En51k5_VERT_COORD[1][j], En51k5_VERT_COORD[2][j])));

   srand(111);
   Tour tour(graph, p, r);//, 100, 50, 0);
   for (std::size_t iter_n = 0; iter_n < p.iterations; ++iter_n) {
      tour.run();
   }

   // graph.print();

   msi::util::GraphElements graphElements{};
   graphElements.translate_vert_into_edges(tour);

   msi::util::Opengl opengl;
   opengl.draw(graphElements);
}

double train(util::IRandomGenerator &rand, Params &params, Graph &graph) {
   srand(time(0));
   Tour tour(graph, params, rand);
   for (std::size_t i = 0; i < params.iterations; ++i) {
      tour.current_iter = i;
      tour.run();
   }
   return tour.min_distance();
}

Graph graph_from_file(Params &params, const std::string &fn_coords, const std::string &fn_demands) {
   msi::util::Reader re;
   std::size_t rows = 51;
   std::size_t cols = 3;

   auto coords = re.read_file(fn_coords, rows, cols);
   cols = 2;
   auto demands = re.read_file(fn_demands, rows, cols);

   Graph graph(params, rows);
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