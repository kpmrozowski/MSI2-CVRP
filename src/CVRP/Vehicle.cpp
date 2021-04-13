#include <MSI/CVRP/Graph.h>
#include <MSI/CVRP/Tour.h>
#include <MSI/CVRP/Vehicle.h>

namespace msi::cvrp {

Vehicle::Vehicle(util::IRandomGenerator &rand, const Params &params, std::size_t vertex_count)
    : m_current_vert(params.depot_id),
      m_params(params),
      m_capacity_left(params.initial_capacity) {}

double Vehicle::traveled_distance(Graph &g) const noexcept {
   double dist = 0;
   for (auto e : m_visited_edges)
      dist += g.m_distance_table[e.first][e.second];
   return dist;
}

VertexId Vehicle::choose_next(Graph &g, util::IRandomGenerator &rand, const std::vector<bool> &feasible_vertices) noexcept {
   VertexId selected_vert = m_current_vert;

   while (selected_vert == m_current_vert) {
      auto prob_target = rand.next_double(g.probability_sum(m_current_vert, m_visited));

      g.for_each_feasible(m_current_vert, feasible_vertices, [this, &prob_target, &selected_vert, prob_sum = 0.0](VertexId id, const Edge &edge) mutable {
         auto visited_it = m_visited.find(id);
         if (visited_it != m_visited.end()) {
            return false;
         }

         prob_sum += edge.prob(m_params);
         if (prob_sum >= prob_target && id != m_current_vert) {
            selected_vert = id;
            return true;
         }

         return false;
      });
   }

   return selected_vert;
}

void Vehicle::reset() noexcept {
   m_visited.clear();
   m_visited_edges.clear();
   m_current_vert = m_params.depot_id;
   m_capacity_left =  m_params.initial_capacity;
}

std::vector<VertexId> Vehicle::route() const noexcept {
   std::vector<VertexId> result(m_visited_edges.size()+1);
   if (m_visited_edges.empty())
      return result;

   std::transform(m_visited_edges.begin(), m_visited_edges.end(), result.begin(), [](const std::pair<VertexId, VertexId> &edge) {
      return edge.first;
   });
   result[m_visited_edges.size()] = m_visited_edges[m_visited_edges.size()-1].second;

   return result;
}

}// namespace msi::cvrp