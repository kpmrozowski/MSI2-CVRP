#include <MSI/CVRP/Graph.h>
#include <fmt/core.h>

namespace msi::cvrp {

Graph::Graph(std::size_t vert_count)
    : m_vertex_count(vert_count),
      m_vertices(vert_count),
      m_distance_table(std::vector<std::vector<double>>(vert_count, std::vector<double>(vert_count))),
      m_edges(vert_count * vert_count) {}

Position::Position(double _x1, double _y1, double _x2, double _y2)
    : x1{_x1}, y1{_y1}, x2{_x2}, y2{_y2} {}

Edge::Edge(double pheromone, Position position)
    : m_position(position),
      m_pheromone(pheromone),
      m_distance(std::pow(std::pow(m_position.x2 - m_position.x1, 2) + std::pow(m_position.y2 - m_position.y1, 2), 0.5)) {}

void Graph::import_vertices(Table coordinates_xy, Table demands) noexcept {
   for (VertexId i = 0; i < coordinates_xy[0].size(); i++) {
      this->m_vertices[i].x = coordinates_xy[1][i];
      this->m_vertices[i].y = coordinates_xy[2][i];
      this->m_vertices[i].demand = demands[1][i];
   }
}

void Graph::compute_distances() noexcept {
   std::vector<double> vt(m_vertex_count);
   for (VertexId i = 0; i < m_vertex_count; i++) {
      for (VertexId j = 0; j < m_vertex_count; j++) {
         this->m_distance_table[j][i] = sqrt(pow(m_vertices[j].x - m_vertices[i].x, 2) + pow(m_vertices[j].y - m_vertices[i].y, 2));
      }
   }
}

void Graph::connect(VertexId a, VertexId b, Edge e) noexcept {
   if (a >= m_vertex_count || b >= m_vertex_count) {
      return;
   }
   m_edges[a * m_vertex_count + b] = e;
   m_edges[b * m_vertex_count + a] = e;
}

void Graph::disconnect(VertexId a, VertexId b) noexcept {
   if (a >= m_vertex_count || b >= m_vertex_count) {
      return;
   }
   m_edges[a * m_vertex_count + b].reset();
   m_edges[b * m_vertex_count + a].reset();
}

void Graph::print() const noexcept {
   for (VertexId i = 0; i < m_vertex_count; ++i) {
      fmt::print("vertex connections {}\n", i);
      for_each_connected(i, [](VertexId id, const Edge &e) {
         fmt::print(" {}\t(fero: {:2.4f},\tdist: {:2.3})\n", id, e.m_pheromone, e.m_distance);
         return false;
      });
   }
}

void Graph::for_each_connected(VertexId vert, const std::function<bool(VertexId, const Edge &)> &callback) const noexcept {
   if (vert >= m_vertex_count) {
      return;
   }

   auto at = m_edges.begin() + vert * m_vertex_count;
   auto end = m_edges.begin() + (vert + 1) * m_vertex_count;
   for (VertexId i = 0; i < m_vertex_count && at != end; ++i) {
      if (at->has_value()) {
         if (callback(i, at->value())) {
            return;
         }
      }
      ++at;
   }
}

void Graph::for_each_feasible(VertexId vertex, const std::vector<bool> &feasible_vertices, const std::function<bool(VertexId, const Edge &)> &callback) const noexcept {
   if (vertex >= m_vertex_count)
      return;

   auto at = m_edges.begin() + vertex * m_vertex_count;
   auto end = m_edges.begin() + (vertex + 1) * m_vertex_count;
   for (VertexId i = 0; i < m_vertex_count && at != end; ++i) {
      if (at->has_value()) {
         if (feasible_vertices[i]) {
            if (callback(i, at->value())) {
               return;
            }
         }
      }
      ++at;
   }
}

void Graph::evaporate(double rate) noexcept {
   for (std::optional<Edge> &edge : m_edges) {
      if (edge.has_value()) {
         edge->m_pheromone *= rate;
      }
   }
}

double Graph::probability_sum(VertexId id, std::set<VertexId> &except, double alpha, double beta) const {
   double result = 0.0;
   for_each_connected(id, [alpha, beta, &result, &except](VertexId id, const Edge &edge) {
      auto visited_it = except.find(id);
      if (visited_it != except.end()) {
         return false;
      }

      result += edge.prob(alpha, beta);
      return false;
   });
   return result;
}

double Graph::pheromone(VertexId a, VertexId b) const {
   return m_edges[a * m_vertex_count + b]->m_pheromone;
}

double Graph::distance(VertexId a, VertexId b) const {
   auto edge = m_edges[a * m_vertex_count + b];
   if (edge.has_value()) {
      return edge->m_distance;
   }
   return std::numeric_limits<double>::infinity();
}

double Graph::distance_overall() const {
   double distance_overall = 0;
   for (auto edge : m_edges)
      if (edge.has_value()) {
         distance_overall += edge->m_distance;
      }
   return distance_overall;
}

void Graph::set_pheromone(VertexId a, VertexId b, double value) {
   m_edges[a * m_vertex_count + b]->m_pheromone = value;
}

void Graph::add_pheromone(VertexId a, VertexId b, double value) {
   m_edges[a * m_vertex_count + b]->m_pheromone += value;
}

double find_shortest_path(Graph &g, VertexId from, VertexId to) {
   std::vector<double> distances(g.vertex_count() * g.vertex_count());
   for (VertexId i = 0; i < g.vertex_count(); ++i) {
      for (VertexId j = 0; j < g.vertex_count(); ++j) {
         if (i == j) {
            distances[i + j * g.vertex_count()] = 0;
            continue;
         }
         distances[i + j * g.vertex_count()] = g.distance(i, j);
      }
   }

   for (VertexId k = 0; k < g.vertex_count(); ++k) {
      for (VertexId i = 0; i < g.vertex_count(); ++i) {
         for (VertexId j = 0; j < g.vertex_count(); ++j) {
            if (distances[i + g.vertex_count() * j] > distances[i + g.vertex_count() * k] + distances[k + g.vertex_count() * j]) {
               distances[i + g.vertex_count() * j] = distances[i + g.vertex_count() * k] + distances[k + g.vertex_count() * j];
            }
         }
      }
   }

   return distances[from + g.vertex_count() * to];
}

}// namespace msi::cvrp
