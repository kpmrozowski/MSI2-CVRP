#include <MSI/AntSystem/Ant.h>

namespace msi::ant_system {

Ant::Ant(util::IRandomGenerator &rand, std::size_t vert_count) : m_current_vert(rand.next_int(vert_count)) {}

void Ant::choose_next(Graph &g, util::IRandomGenerator &rand) noexcept {
   auto prob_target = rand.next_double(g.probability_sum(m_current_vert, m_visited));
   VertId selected_vert = m_current_vert;

   g.for_each_connected(m_current_vert, [this, &prob_target, &selected_vert, prob_sum = 0.0](VertId id, const Edge &edge) mutable {
     auto visited_it = m_visited.find(id);
     if (visited_it != m_visited.end()) {
        return false;
     }

     prob_sum += edge.prob();
     if (prob_sum >= prob_target) {
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

}// namespace msi::ant_system
