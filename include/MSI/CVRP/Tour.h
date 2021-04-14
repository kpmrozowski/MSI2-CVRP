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
   double m_min_distance = std::numeric_limits<double>::infinity();
   std::vector<VertexId> m_min_route;

 public:
   Graph &m_graph;
   std::vector<Vehicle> m_vehicles;
   std::vector<std::size_t> m_ant_completed;
   std::vector<bool> m_unvisited_verts;
   std::size_t current_iter;

   Tour(Graph &graph,
        Params &params,
        util::IRandomGenerator &rand);

   void reset_vehicles() noexcept;
   void run(std::size_t iter) noexcept;
   void update_pheromone() noexcept;
   void run_elite(std::size_t iter) noexcept;
   [[nodiscard]] std::pair<double, std::size_t> shortest_distance() noexcept;
   [[nodiscard]] const Vehicle &best_vehicle() const noexcept;

   [[nodiscard]] constexpr double min_distance() const noexcept {
      return m_min_distance;
   };
};

}// namespace msi::cvrp

#endif//CVRP_TOUR_H
