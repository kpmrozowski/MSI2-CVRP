#ifndef CVRP_VEHICLE_H
#define CVRP_VEHICLE_H
#include "../Util/IRandomGenerator.h"
#include "Graph.h"
#include "Params.h"
#include <set>
#include <vector>

namespace msi::cvrp {

class Vehicle {
 public:
   std::set<VertexId> m_visited{};
   std::vector<std::pair<VertexId, VertexId>> m_visited_edges{};
   VertexId m_current_vert;
   const Params &m_params;

 public:
   double m_capacity_left;

   Vehicle(util::IRandomGenerator &rand, const Params &params, std::size_t vertex_count);

   [[nodiscard]] double traveled_distance(Graph &g) const noexcept;
   [[nodiscard]] VertexId choose_next(Graph &g, util::IRandomGenerator &rand, const std::vector<bool> &feasible_vertices) noexcept;
   void reset() noexcept;

   [[nodiscard]] constexpr VertexId current_vert() const noexcept {
      return m_current_vert;
   }

   [[nodiscard]] constexpr const std::vector<std::pair<VertexId, VertexId>> &edges() const noexcept {
      return m_visited_edges;
   }
};

}// namespace msi::cvrp

#endif//CVRP_VEHICLE_H