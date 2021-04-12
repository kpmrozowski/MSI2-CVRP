#ifndef CVRP_GRAPH_H
#define CVRP_GRAPH_H
#include "Params.h"
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
   Edge(double pheromone, Position pos);
   Position m_position;
   double m_pheromone;
   double m_distance;

   [[nodiscard]] constexpr double prob(const Params &params) const noexcept {
      double alpha = params.alpha_final + (params.current_iteration - 0) * (params.alpha_final - params.alpha_initial)/(params.iterations - 0);
      double beta = params.beta_initial + (params.current_iteration - 0) * (params.beta_final - params.beta_initial)/(params.iterations - 0);
      return std::pow(m_pheromone, alpha) * std::pow(1.0 / m_distance, beta);
   }
};

using VertexId = std::size_t;
using Table = std::vector<std::vector<double>>;

class Graph {
   std::size_t m_vertex_count;

 public:
   std::vector<Vertice> m_vertices;
   std::vector<std::vector<double>> m_distance_table;
   std::vector<std::optional<Edge>> m_edges;
   const Params &m_params;

   explicit Graph(const Params &params, std::size_t vert_count);

   void import_vertices(Table coordinates_xy, Table demands) noexcept;
   void compute_distances() noexcept;
   void connect(VertexId a, VertexId b, Edge e) noexcept;
   void disconnect(VertexId a, VertexId b) noexcept;
   void print() const noexcept;
   void for_each_connected(VertexId vert, const std::function<bool(VertexId, const Edge &)> &callback) const noexcept;
   void for_each_feasible(VertexId vertex, const std::vector<bool> &feasible_vertices, const std::function<bool(VertexId, const Edge &)> &callback) const noexcept;
   void evaporate(std::size_t current_iter) noexcept;
   void set_pheromone(VertexId a, VertexId b, double value);
   void add_pheromone(VertexId a, VertexId b, double value);
   [[nodiscard]] double probability_sum(VertexId id, std::set<VertexId> &except) const;
   [[nodiscard]] double pheromone(VertexId a, VertexId b) const;
   [[nodiscard]] double distance(VertexId a, VertexId b) const;
   [[nodiscard]] double distance_overall() const;

   [[nodiscard]] constexpr std::size_t vertex_count() const noexcept {
      return m_vertex_count;
   }
};

}// namespace msi::cvrp

#endif//CVRP_GRAPH_H
