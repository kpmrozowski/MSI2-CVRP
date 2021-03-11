#ifndef CVRP_ANT_H
#define CVRP_ANT_H
#include "Graph.h"
#include "../Util/IRandomGenerator.h"
#include <vector>

namespace msi::ant_system {

class Ant {
   std::set<VertId> m_visited;
   std::vector<std::pair<VertId, VertId>> m_visited_edges;
   VertId m_current_vert;

 public:
   Ant(util::IRandomGenerator &rand, std::size_t vert_count);

   void choose_next(Graph &g, util::IRandomGenerator &rand) noexcept;

   [[nodiscard]] constexpr VertId current_vert() const noexcept {
      return m_current_vert;
   }

   [[nodiscard]] constexpr const std::vector<std::pair<VertId, VertId>> &edges() const noexcept {
      return m_visited_edges;
   }
};

}

#endif//CVRP_ANT_H
