#ifndef CVRP_TOUR_H
#define CVRP_TOUR_H
#include "../Util/IRandomGenerator.h"
#include "Vechicle.h"
#include "Graph.h"
#include <memory>
#include <numeric>

namespace msi::cvrp {

class Tour {
  util::IRandomGenerator &m_rand;
  std::size_t m_passes;
  VertId m_target;

 public:
  Graph &m_graph;
  std::vector<std::unique_ptr<Vechicle>> m_vechicles;
  std::vector<std::size_t> m_ant_completed;
  std::vector<bool> m_unvisited_verts;

  Tour( Graph &graph,
        util::IRandomGenerator &rand,
        std::size_t ant_count = g_n_ants,
        std::size_t passes = g_n_passes,
        VertId target = g_depot_id
        );
  
  void reset_vechicles() noexcept;
  void run() noexcept;
  double shortest_distance() noexcept;
  Vechicle best_vechicle() noexcept;
  void update_pheromone() noexcept;
};

}// namespace msi::cvrp

#endif//CVRP_TOUR_H
