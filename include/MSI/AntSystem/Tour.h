#ifndef ANT_TOUR_H
#define ANT_TOUR_H
#include "../Util/IRandomGenerator.h"
#include "Ant.h"
#include "Graph.h"
#include <memory>
#include <numeric>

namespace msi::ant_system {

class Tour {
   Graph &m_graph;
   util::IRandomGenerator &m_rand;
   std::vector<std::unique_ptr<Ant>> m_ants;
   std::size_t m_passes;
   VertId m_target;

 public:
   Tour(Graph &graph, util::IRandomGenerator &rand, std::size_t ant_count, std::size_t passes, VertId target);

   void reset_vechicles() noexcept;
   void run() noexcept;
};

}// namespace msi::ant_system

#endif//ANT_TOUR_H
