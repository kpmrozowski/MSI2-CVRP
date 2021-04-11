#ifndef CVRP_TOUR_H
#define CVRP_TOUR_H
#include "../Util/IRandomGenerator.h"
#include "Graph.h"
#include "Params.h"
#include "Vehicle.h"
#include <memory>
#include <numeric>

namespace msi::cvrp {

class Tour {
   util::IRandomGenerator &m_rand;
   Params &m_params;
   std::size_t m_passes;
   VertexId m_target;

 public:
   Graph &m_graph;
   std::vector<Vehicle> m_vehicles;
   std::vector<std::size_t> m_ant_completed;
   std::vector<bool> m_unvisited_verts;

   Tour(Graph &graph,
        Params &params,
        util::IRandomGenerator &rand);

   void reset_vehicles() noexcept;
   void run() noexcept;
   void update_pheromone() noexcept;
   [[nodiscard]] double shortest_distance() noexcept;
   [[nodiscard]] const Vehicle &best_vehicle() const noexcept;
};

}// namespace msi::cvrp

#endif//CVRP_TOUR_H
