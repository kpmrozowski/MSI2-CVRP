#include <MSI/AntSystem/Graph.h>
#include <fmt/core.h>

namespace msi::ant_system {

Graph::Graph(std::size_t vert_count) : m_edges(vert_count * vert_count), m_vert_count(vert_count) {}

Position::Position(double _x1, double _y1, double _x2, double _y2)
   : x1{_x1}
   , y1{_y1}
   , x2{_x2}
   , y2{_y2} {}

Edge::Edge(double _pheromone, double _distance, Position _position)
: pheromone{_pheromone}
, distance{_distance}
, pos{_position} {
   distance = std::pow(std::pow(pos.x2 - pos.x1, 2) + std::pow(pos.y2 - pos.y1, 2), 0.5);
}

void Graph::connect(VertId a, VertId b, Edge e) noexcept {
   if (a >= m_vert_count || b >= m_vert_count) {
      return;
   }
   // fmt::print("x1={:2.1f} x2={:2.1f} y1={:2.1f} y2={:2.1f} dist={:2.1f}\n", e.pos.x2, e.pos.x1, e.pos.y2, e.pos.y1, e.distance);
   m_edges[a * m_vert_count + b] = e;
   m_edges[b * m_vert_count + a] = e;
}

void Graph::print() const noexcept {
   for (VertId i = 0; i < m_vert_count; ++i) {
      fmt::print("vertex connections {}\n", i);
      for_each_connected(i, [](VertId id, const Edge &e) {
         fmt::print(" {}\t(fero: {:2.4f},\tdist: {:2.3})\n", id, e.pheromone, e.distance);
         // fmt::print("x1={:2.1f} x2={:2.1f} y1={:2.1f} y2={:2.1f} dist={:2.1f}\n", e.pos.x2, e.pos.x1, e.pos.y2, e.pos.y1, e.distance);
         return false;
      });
   }
}

void Graph::for_each_connected(VertId vert, const std::function<bool(VertId, const Edge &)> &callback) const noexcept {
   if (vert >= m_vert_count) {
      return;
   }

   auto at = m_edges.begin() + vert * m_vert_count;
   auto end = m_edges.begin() + (vert + 1) * m_vert_count;
   for (VertId i = 0; i < m_vert_count && at != end; ++i) {
      if (at->has_value()) {
         if (callback(i, at->value())) {
            return;
         }
      }
      ++at;
   }
}

void Graph::evaporate() noexcept {
   for (std::optional<Edge> &edge : m_edges) {
      if (edge.has_value()) {
         edge->pheromone *= g_evapor;
      }
   }
}

double Graph::probability_sum(VertId id, std::set<VertId> &except) const {
   double result = 0.0;
   for_each_connected(id, [&result, &except](VertId id, const Edge &edge) {
      auto visited_it = except.find(id);
      if (visited_it != except.end()) {
         return false;
      }

      result += edge.prob();
      return false;
   });
   return result;
}

double Graph::pheromone(VertId a, VertId b) const {
   return m_edges[a * m_vert_count + b]->pheromone;
}

double Graph::distance(VertId a, VertId b) const {
   auto edge = m_edges[a * m_vert_count + b];
   if (edge.has_value()) {
      return edge->distance;
   }
   return std::numeric_limits<double>::infinity();
}

void Graph::set_pheromone(VertId a, VertId b, double value) {
   m_edges[a * m_vert_count + b]->pheromone = value;
}

void Graph::add_pheromone(VertId a, VertId b, double value) {
   m_edges[a * m_vert_count + b]->pheromone += value;
}

double find_shortest_path(Graph &g, VertId from, VertId to) {
   std::vector<double> distances(g.vert_count() * g.vert_count());
   for (VertId i = 0; i < g.vert_count(); ++i) {
      for (VertId j = 0; j < g.vert_count(); ++j) {
         if (i == j) {
            distances[i + j * g.vert_count()] = 0;
            continue;
         }
         distances[i + j * g.vert_count()] = g.distance(i, j);
      }
   }

   for (VertId k = 0; k < g.vert_count(); ++k) {
      for (VertId i = 0; i < g.vert_count(); ++i) {
         for (VertId j = 0; j < g.vert_count(); ++j) {
            if (distances[i + g.vert_count() * j] > distances[i + g.vert_count() * k] + distances[k + g.vert_count() * j]) {
               distances[i + g.vert_count() * j] = distances[i + g.vert_count() * k] + distances[k + g.vert_count() * j];
            }
         }
      }
   }

   return distances[from + g.vert_count() * to];
}

}// namespace msi::ant_system
