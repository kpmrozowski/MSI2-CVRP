#ifndef CVRP_GRAPH_H
#define CVRP_GRAPH_H
#include "Constants.h"
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <vector>

namespace msi::ant_system {

struct Edge {
   double pheromone;
   double distance;

   [[nodiscard]] constexpr double prob() const noexcept {
      return std::pow(pheromone, g_alpha) * std::pow(1.0 / distance, g_beta);
   }
};

using VertId = std::size_t;

class Graph {
   std::vector<std::optional<Edge>> m_edges;
   std::size_t m_vert_count;

 public:
   explicit Graph(std::size_t vert_count);

   void connect(VertId a, VertId b, Edge e) noexcept;
   void print() const noexcept;
   void for_each_connected(VertId vert, const std::function<bool(VertId, const Edge &)> &callback) const noexcept;
   void evaporate() noexcept;
   void set_pheromone(VertId a, VertId b, double value);
   [[nodiscard]] double find_prob_sum(VertId id, std::set<VertId> &except) const;
   [[nodiscard]] double pheromone(VertId a, VertId b) const;
   [[nodiscard]] double distance(VertId a, VertId b) const;

   [[nodiscard]] constexpr std::size_t vert_count() const noexcept {
      return m_vert_count;
   }
};

}// namespace msi::ant_system

#endif//CVRP_GRAPH_H
