#include <MSI/AntSystem/Graph.h>
#include <MSI/AntSystem/Tour.h>
#include <MSI/Util/IRandomGenerator.h>
#include <gtest/gtest.h>

class Random : public msi::util::IRandomGenerator {
 public:
   double next_double(double max) noexcept override {
      return static_cast<double>(std::rand() % 10000) * max / 10000.0;
   }

   int next_int(int max) noexcept override {
      return std::rand() % max;
   }
};

TEST(AntSystem, BasicGraph) {
   Random r;

   msi::ant_system::Graph graph(7);
   graph.connect(0, 1, {1.0, 5.0});
   graph.connect(0, 2, {1.0, 6.0});
   graph.connect(2, 3, {1.0, 2.0});
   graph.connect(1, 4, {1.0, 3.0});
   graph.connect(4, 5, {1.0, 2.5});
   graph.connect(1, 6, {1.0, 1.5});
   graph.connect(6, 5, {1.0, 1.5});

   msi::ant_system::Tour tour(graph, r, 100, 6, 5);
   for (std::size_t i = 0; i < 10; ++i) {
      tour.run();
      tour.reset();
   }

   graph.print();

   ASSERT_GT(graph.pheromone(0, 1), graph.pheromone(0, 2));
   ASSERT_GT(graph.pheromone(1, 6), graph.pheromone(1, 4));
   ASSERT_GT(graph.pheromone(1, 4), graph.pheromone(1, 0));
   ASSERT_GT(graph.pheromone(2, 0), graph.pheromone(2, 3));
   ASSERT_GT(graph.pheromone(6, 5), graph.pheromone(6, 1));
   ASSERT_GT(graph.pheromone(4, 5), graph.pheromone(4, 1));
}