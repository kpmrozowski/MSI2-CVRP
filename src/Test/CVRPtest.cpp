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
   Random r;
   msi::cvrp::Reader re;
   std::size_t rows = 51;
	std::size_t cols = 3;
	double **En51k5_NODE_COORD, **En51k5_DEMANDS;
	En51k5_NODE_COORD = re.read_file("./src/CVRP/E-n51-k5_NODE_COORD.txt",rows,cols);
   cols = 2;
	En51k5_DEMANDS = re.read_file("./src/CVRP/E-n51-k5_DEMANDS.txt",rows,cols);
   msi::ant_system::Graph graph(rows);
   // for(std::size_t i = 0; i < rows; i++)
   //    for(std::size_t j = i + 1; j < rows; j++)   
   //       graph.connect(i, j, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(En51k5_NODE_COORD[1][i], En51k5_NODE_COORD[2][i], En51k5_NODE_COORD[1][j], En51k5_NODE_COORD[2][j])));

   for(std::size_t i = 0; i < rows-1; i++)
      graph.connect(i, i+1, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(En51k5_NODE_COORD[1][i], En51k5_NODE_COORD[2][i], En51k5_NODE_COORD[1][i+1], En51k5_NODE_COORD[2][i+1])));
   
   srand(111);
   msi::ant_system::Tour tour(graph, r, 100, 50, 0);
   for (std::size_t i = 0; i < 100; ++i) {
      tour.run();
      tour.reset();
   }

   graph.print();
   msi::cvrp::Vertices vertices;
   vertices.translate_vert_into_edges(graph);

   msi::cvrp::Opengl opengl;
   opengl.draw(vertices);

}