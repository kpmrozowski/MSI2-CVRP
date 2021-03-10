#include <cmath>
#include <fmt/core.h>
#include <functional>
#include <numeric>
#include <optional>
#include <set>
#include <vector>

namespace msi::ant_system {

constexpr double g_alpha = 1.0;
constexpr double g_beta = 0.8;
constexpr double g_evapor = 0.95;

struct Edge {
   double pheromone;
   double distance;

   [[nodiscard]] constexpr double prob() const noexcept {
      return std::pow(pheromone, g_alpha) * std::pow(1.0 / distance, g_beta);
   }
};

using VertId = std::size_t;

class Graph {
   std::vector<std::optional<Edge>> m_edges;
   std::size_t m_vert_count;

 public:
   explicit Graph(std::size_t vert_count) : m_edges(vert_count * vert_count), m_vert_count(vert_count) {}

   void connect(VertId a, VertId b, Edge e) noexcept {
      if (a >= m_vert_count || b >= m_vert_count) {
         return;
      }
      m_edges[a * m_vert_count + b] = e;
      m_edges[b * m_vert_count + a] = e;
   }

   void print() const noexcept {
      for (VertId i = 0; i < m_vert_count; ++i) {
         fmt::print("vertex connections {}\n", i);
         for_each_connected(i, [](VertId id, const Edge &e) {
            fmt::print(" {} (fer: {})\n", id, e.pheromone);
            return false;
         });
      }
   }

   void for_each_connected(VertId vert, const std::function<bool(VertId, const Edge &)> &callback) const noexcept {
      if (vert >= m_vert_count) {
         return;
      }

      auto at = m_edges.begin() + vert * m_vert_count;
      auto end = m_edges.begin() + (vert + 1) * m_vert_count;
      for (VertId i = 0; i < m_vert_count && at != end; ++i) {
         if (at->has_value()) {
            if (callback(i, at->value())) {
               return;
            }
         }
         ++at;
      }
   }

   void evaporate() noexcept {
      for (std::optional<Edge> &edge : m_edges) {
         if (edge.has_value()) {
            edge->pheromone *= g_evapor;
         }
      }
   }

   [[nodiscard]] constexpr std::size_t vert_count() const noexcept {
      return m_vert_count;
   }

   [[nodiscard]] double find_prob_sum(VertId id, std::set<VertId> &except) const {
      double result = 0.0;
      for_each_connected(id, [&result, &except](VertId id, const Edge &edge) {
         auto visited_it = except.find(id);
         if (visited_it != except.end()) {
            return false;
         }

         result += edge.prob();
         return false;
      });
      return result;
   }

   [[nodiscard]] double pheromone(VertId a, VertId b) const {
      return m_edges[a * m_vert_count + b]->pheromone;
   }

   [[nodiscard]] double distance(VertId a, VertId b) const {
      return m_edges[a * m_vert_count + b]->distance;
   }

   void set_pheromone(VertId a, VertId b, double value) {
      m_edges[a * m_vert_count + b]->pheromone = value;
      m_edges[b * m_vert_count + a]->pheromone = value;
   }
};

class IRandomGenerator {
 public:
   [[nodiscard]] virtual double next_double(double max) noexcept = 0;
   [[nodiscard]] virtual int next_int(int max) noexcept = 0;
};

class Ant {
   std::set<VertId> m_visited;
   std::vector<std::pair<VertId, VertId>> m_visited_edges;
   VertId m_current_vert;

 public:
   Ant(IRandomGenerator &rand, std::size_t vert_count) : m_current_vert(rand.next_int(vert_count)) {}

   void choose_next(Graph &g, IRandomGenerator &rand) noexcept {
      auto prob_target = rand.next_double(g.find_prob_sum(m_current_vert, m_visited));
      VertId selected_vert = m_current_vert;

      g.for_each_connected(m_current_vert, [this, &prob_target, &selected_vert, probSum = 0.0](VertId id, const Edge &edge) mutable {
         auto visited_it = m_visited.find(id);
         if (visited_it != m_visited.end()) {
            return false;
         }

         probSum += edge.prob();
         if (probSum >= prob_target) {
            selected_vert = id;
            return true;
         }

         return false;
      });

      if (selected_vert == m_current_vert) {
         return;
      }

      m_visited_edges.emplace_back(m_current_vert, selected_vert);
      m_visited.insert(selected_vert);
      m_current_vert = selected_vert;
   }

   [[nodiscard]] constexpr VertId current_vert() const noexcept {
      return m_current_vert;
   }

   [[nodiscard]] constexpr const std::vector<std::pair<VertId, VertId>> &edges() const noexcept {
      return m_visited_edges;
   }
};

class Tour {
   Graph &m_graph;
   IRandomGenerator &m_rand;
   std::vector<std::unique_ptr<Ant>> m_ants;
   std::size_t m_passes;
   VertId m_target;

 public:
   Tour(Graph &graph, IRandomGenerator &rand, std::size_t ant_count, std::size_t passes, VertId target) : m_graph(graph), m_rand(rand), m_ants(ant_count), m_passes(passes), m_target(target) {
      std::generate(m_ants.begin(), m_ants.end(), [&rand, count = graph.vert_count()]() {
         return std::make_unique<Ant>(rand, count);
      });
   }

   void reset() noexcept {
      std::generate(m_ants.begin(), m_ants.end(), [this]() {
         return std::make_unique<Ant>(m_rand, m_graph.vert_count());
      });
   }

   void run() noexcept {
      for (std::size_t pass = 0; pass < m_passes; ++pass) {
         for (std::unique_ptr<Ant> &ant : m_ants) {
            if (ant->current_vert() == m_target) {
               continue;
            }
            ant->choose_next(m_graph, m_rand);
         }
      }

      m_graph.evaporate();

      for (std::unique_ptr<Ant> &ant : m_ants) {
         if (ant->current_vert() != m_target) {
            continue;
         }

         auto &edges = ant->edges();
         auto total_distance = std::accumulate(edges.begin(), edges.end(), 0.0, [this](double acc, const std::pair<VertId, VertId> &pair) {
            return acc + m_graph.distance(pair.first, pair.second);
         });

         for (const auto &pair : edges) {
            m_graph.set_pheromone(pair.first, pair.second, m_graph.pheromone(pair.first, pair.second) + 1.0 / total_distance);
         }
      }
   }
};

}// namespace msi::ant_system

class Random : public msi::ant_system::IRandomGenerator {
 public:
   double next_double(double max) noexcept override {
      return static_cast<double>(std::rand() % 10000) * max / 10000.0;
   }

   int next_int(int max) noexcept override {
      return std::rand() % max;
   }
};

int main() {
   Random r;

   msi::ant_system::Graph graph(7);
   graph.connect(0, 1, {1.0, 5.0});
   graph.connect(0, 2, {1.0, 6.0});
   graph.connect(2, 3, {1.0, 2.0});
   graph.connect(1, 4, {1.0, 3.0});
   graph.connect(4, 5, {1.0, 2.5});
   graph.connect(1, 6, {1.0, 1.5});
   graph.connect(6, 5, {1.0, 1.5});

   msi::ant_system::Tour tour(graph, r, 10, 5, 5);
   for (std::size_t i = 0; i < 10; ++i) {
      tour.run();
      tour.reset();
   }

   graph.print();
   return 0;
}