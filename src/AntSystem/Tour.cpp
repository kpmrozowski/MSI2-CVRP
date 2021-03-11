#include <MSI/AntSystem/Tour.h>

namespace msi::ant_system {

Tour::Tour(Graph &graph, util::IRandomGenerator &rand, std::size_t ant_count, std::size_t passes, VertId target) : m_graph(graph), m_rand(rand), m_ants(ant_count), m_passes(passes), m_target(target) {
   std::generate(m_ants.begin(), m_ants.end(), [&rand, count = graph.vert_count()]() {
     return std::make_unique<Ant>(rand, count);
   });
}

void Tour::reset() noexcept {
   std::generate(m_ants.begin(), m_ants.end(), [this]() {
     return std::make_unique<Ant>(m_rand, m_graph.vert_count());
   });
}

void Tour::run() noexcept {
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

}