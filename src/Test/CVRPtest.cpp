#include <MSI/CVRP/drawGL.h>
#include <MSI/AntSystem/Graph.h>
#include <MSI/AntSystem/Tour.h>
#include <MSI/Util/IRandomGenerator.h>
#include <gtest/gtest.h>
#include <MSI/CVRP/ReadGraph.h>

class Random : public msi::util::IRandomGenerator {
 public:
   double next_double(double max) noexcept override {
      return static_cast<double>(std::rand() % 10000) * max / 10000.0;
   }

   int next_int(int max) noexcept override {
      return std::rand() % max;
   }
};

TEST(CVRP, ReadGraph) {
   
   using Table = std::vector<std::vector<double>>;
   Random r;
   msi::cvrp::Reader re;
   // re.display_file();
   std::size_t rows = 51;
	std::size_t cols = 3;
	Table En51k5_NODE_COORD, En51k5_DEMANDS;
	En51k5_NODE_COORD = re.read_file("./src/CVRP/E-n51-k5_NODE_COORD.txt",rows,cols);
   cols = 2;
	En51k5_DEMANDS = re.read_file("./src/CVRP/E-n51-k5_DEMANDS.txt",rows,cols);
   msi::ant_system::Graph graph(rows);
   graph.import_vertices(En51k5_NODE_COORD, En51k5_DEMANDS);
   graph.compute_distances();
   for(std::size_t i = 0; i < rows; i++)
      for(std::size_t j = i + 1; j < rows; j++)   
         graph.connect(i, j, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(En51k5_NODE_COORD[1][i], En51k5_NODE_COORD[2][i], En51k5_NODE_COORD[1][j], En51k5_NODE_COORD[2][j])));

   // for(std::size_t i = 0; i < rows; i++)
      // graph.connect(i, i+1, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(En51k5_NODE_COORD[1][i], En51k5_NODE_COORD[2][i], En51k5_NODE_COORD[1][i+1], En51k5_NODE_COORD[2][i+1])));
   
   // graph.disconnect(1, 2);

   srand(111);
   msi::ant_system::Tour tour(graph, r, 100, 50, 0);
   for (std::size_t i = 0; i < 100; ++i) {
      tour.run();
      tour.reset();
   }

   graph.print();
   fmt::print("distance_overall = {:2.4f}\n", graph.distance_overall());
   fmt::print("size = {}\n", En51k5_NODE_COORD[0].size());
   
   msi::cvrp::GraphElements graphElements;
   graphElements.translate_vert_into_edges(graph);

   msi::cvrp::Opengl opengl;
   opengl.draw(graphElements);
}