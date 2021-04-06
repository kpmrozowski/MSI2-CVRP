#ifndef CVRP_VECHICLE_H
#define CVRP_VECHICLE_H
#include "Graph.h"
#include "../Util/IRandomGenerator.h"
#include <vector>

namespace msi::cvrp {

class Vechicle {
  public:
  
  std::set<VertId> m_visited;
  std::vector<std::pair<VertId, VertId>> m_visited_edges;
  VertId m_current_vert;

public:
  double m_capacity_left;

  Vechicle(util::IRandomGenerator &rand, std::size_t vert_count);
  Vechicle& operator=(Vechicle& vech);
  
  double traveled_distance(Graph &g) noexcept;

  VertId choose_next(Graph &g, util::IRandomGenerator &rand, std::vector<bool> feasible_verts) noexcept;
  
  [[nodiscard]] constexpr VertId current_vert() const noexcept {
     return m_current_vert;
  }
  
  [[nodiscard]] constexpr const std::vector<std::pair<VertId, VertId>> &edges() const noexcept {
     return m_visited_edges;
  }
};

} // namespace msi::cvrp

#endif //CVRP_VECHICLE_H