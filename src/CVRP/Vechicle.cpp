#include <MSI/CVRP/Vechicle.h>
#include <fmt/core.h>
#include <MSI/CVRP/Graph.h>

namespace msi::cvrp {

Vechicle::Vechicle(util::IRandomGenerator &rand, std::size_t vert_count)
   : m_current_vert(g_depot_id)
   , m_capacity_left(g_init_capacity)
   // , m_current_vert(rand.next_int(vert_count))
   {}

Vechicle& Vechicle::operator=(Vechicle& vech) {
   m_visited_edges = vech.m_visited_edges;
      return *this;
}

double Vechicle::traveled_distance(Graph &g) noexcept {
   double dist = 0;
   for(auto e : m_visited_edges)
      dist += g.m_distance_table[e.first][e.second];
   return dist;
}

VertId Vechicle::choose_next(Graph &g, util::IRandomGenerator &rand, std::vector<bool> feasible_verts) noexcept {
   auto prob_target = rand.next_double(g.probability_sum(m_current_vert, m_visited));
   VertId selected_vert = m_current_vert;

   g.for_each_feasible(m_current_vert, feasible_verts, [this, &feasible_verts, &prob_target, &selected_vert, prob_sum = 0.0](VertId id, const Edge &edge) mutable {
   //   if (!feasible_verts[id]){
   //      return false;
   //   }

     auto visited_it = m_visited.find(id);
     if (visited_it != m_visited.end()) {
        return false;
     }

     prob_sum += edge.prob();
     if (prob_sum >= prob_target && id != m_current_vert) {
        selected_vert = id;
        return true;
     }

     return false;
   });

   if (selected_vert == m_current_vert) {
      return this->choose_next(g, rand, feasible_verts);
   }
   return selected_vert;
}

} // namespace msi::cvrp 