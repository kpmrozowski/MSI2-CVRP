#include <MSI/Util/drawGL.h>
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
   graph.connect(0, 1, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(0.0, 0.0, 3.0, 4.0)));
   graph.connect(0, 2, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(0.0, 0.0, 6.0, 0.0)));
   graph.connect(2, 3, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(6.0, 0.0, 6.0, 2.0)));
   graph.connect(1, 4, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(3.0, 4.0, 4.0, 1.0)));
   graph.connect(4, 5, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(3.0, 1.0, 1.0, 2.5)));
   graph.connect(1, 6, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(3.0, 4.0, 3.0, 2.5)));
   graph.connect(6, 5, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(3.0, 2.5, 1.0, 2.5)));

   msi::ant_system::Tour tour(graph, r, 100, 6, 5);
   for (std::size_t i = 0; i < 10; ++i) {
      tour.run();
      tour.reset_vechicles();
   }

   ASSERT_GT(graph.pheromone(0, 1), graph.pheromone(0, 2));
   ASSERT_GT(graph.pheromone(1, 6), graph.pheromone(1, 4));
   ASSERT_GT(graph.pheromone(1, 4), graph.pheromone(1, 0));
   ASSERT_GT(graph.pheromone(2, 0), graph.pheromone(2, 3));
   ASSERT_GT(graph.pheromone(6, 5), graph.pheromone(6, 1));
   ASSERT_GT(graph.pheromone(4, 5), graph.pheromone(4, 1));
}

static std::size_t find_path_length(msi::ant_system::Graph &graph, msi::ant_system::VertId starting_vertex) {
   msi::ant_system::VertId current_vertex = starting_vertex;
   std::size_t path_length = 0;
   while (current_vertex != 0) {
      msi::ant_system::VertId best_neighbour = -1;
      double best_fer_value = 0.0;
      graph.for_each_connected(current_vertex, [&best_neighbour, &best_fer_value](msi::ant_system::VertId vertex, const msi::ant_system::Edge &edge) {
        if (edge.pheromone > best_fer_value) {
           best_fer_value = edge.pheromone;
           best_neighbour = vertex;
        }
        return false;
      });
      current_vertex = best_neighbour;
      ++path_length;
   }
   return path_length;
}

TEST(AntSystem, RandomGraph) {
   srand(800);
   Random r;

   msi::ant_system::Graph graph(50);
   for (std::size_t i = 0; i < 200; ++i) {
      // tutaj 200 razy losujesz polaczenia miedzy 50 wierzchołkami, ale ze zwracaniem, czyli część wierzchołków jest niepolaczonych...
      graph.connect(r.next_int(50), r.next_int(50), msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(r.next_double(10.0), r.next_double(10.0), r.next_double(10.0), r.next_double(10.0))));
   }

   srand(111);
   msi::ant_system::Tour tour(graph, r, 100, 50, 0);
   for (std::size_t i = 0; i < 100; ++i) {
      tour.run();
      tour.reset_vechicles();
   }

   graph.print();

   ASSERT_EQ(find_path_length(graph, 49), 2);
   ASSERT_EQ(find_path_length(graph, 30), 3);
   ASSERT_EQ(find_path_length(graph, 2), 4);
}

TEST(AntSystem, Hub) {
   srand(800);
   Random r;

   msi::ant_system::Graph graph(10);
   // 1 is a hub that is connected to 0
   graph.connect(0, 1, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(0.0, 1.0, 2.25, 1.0)));
   // 2-8 are connected to 1
   graph.connect(1, 2, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(2.25, 1.0, 4.25, 1.0)));
   graph.connect(1, 3, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(2.25, 1.0, 2.25, 2.0)));
   graph.connect(1, 4, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(2.25, 1.0, 3.25, 4.0)));
   graph.connect(1, 5, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(2.25, 1.0, 4.25, 2.0)));
   graph.connect(1, 6, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(2.25, 1.0, 4.25, 3.0)));
   graph.connect(1, 7, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(2.25, 1.0, 5.25, 2.0)));
   graph.connect(1, 8, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(2.25, 1.0, 5.25, 3.0)));
   // 9 is connected to 0 and to 2 one
   graph.connect(9, 0, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(0.5, 5.0, 0.0, 1.0)));
   graph.connect(9, 1, msi::ant_system::Edge(1.0, 0.0, msi::ant_system::Position(0.5, 5.0, 2.25, 1.0)));

   msi::ant_system::Tour tour(graph, r, 20, 10, 0);
   for (std::size_t i = 0; i < 100; ++i) {
      tour.run();
      tour.reset_vechicles();
   }

   graph.print();

   // we want ants from vertex 9 to go directly to 0 rather than going though a hub
   ASSERT_GT(graph.pheromone(9, 0), graph.pheromone(9, 1));
}
