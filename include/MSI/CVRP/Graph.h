#ifndef CVRP_GRAPH_H
#define CVRP_GRAPH_H
#include "Constants.h"
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <vector>

namespace msi::cvrp {

struct Position {
   Position(double _x1, double _y1, double _x2, double _y2);
   double x1, y1, x2, y2;
};

struct Vertice {
   double x, y, demand;
};

struct Edge {
   Edge(double _pheromone, Position _pos);
   double pheromone;
   double distance;
   Position pos;

   [[nodiscard]] constexpr double prob() const noexcept {
      // return std::pow(pheromone, g_alpha + (iter)*(g_alpha_max - g_alpha)/(iter_max)) * std::pow(1.0 / distance, g_beta);
      return std::pow(pheromone, g_alpha) * std::pow(1.0 / distance, g_beta);
   }
};

using VertId = std::size_t;
using Table = std::vector<std::vector<double>>;

class Graph {
   std::size_t m_vert_count;
 public:
   std::vector<Vertice> m_vertices;
   std::vector<std::vector<double>> m_distance_table;
   std::vector<std::optional<Edge>> m_edges;
   // std::set<VertId> feasible_verts_set;

   explicit Graph(std::size_t vert_count);

   void import_vertices(Table coordinates_xy, Table demands) noexcept;
   void compute_distances() noexcept;
   void connect(VertId a, VertId b, Edge e) noexcept;
   void disconnect(VertId a, VertId b) noexcept;
   void print() const noexcept;
   void for_each_connected(VertId vert, const std::function<bool(VertId, const Edge &)> &callback) const noexcept;
   void for_each_feasible(VertId vert, std::vector<bool>& feasible_verts, const std::function<bool(VertId, const Edge &)> &callback) const noexcept;
   void evaporate() noexcept;
   void set_pheromone(VertId a, VertId b, double value);
   void add_pheromone(VertId a, VertId b, double value);
   [[nodiscard]] double probability_sum(VertId id, std::set<VertId> &except) const;
   [[nodiscard]] double pheromone(VertId a, VertId b) const;
   [[nodiscard]] double distance(VertId a, VertId b) const;
   [[nodiscard]] double distance_overall() const;

   [[nodiscard]] constexpr std::size_t vert_count() const noexcept {
      return m_vert_count;
   }
};

}// namespace msi::cvrp

#endif//CVRP_GRAPH_H
