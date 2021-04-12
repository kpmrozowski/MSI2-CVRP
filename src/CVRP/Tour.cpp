#include <MSI/CVRP/Tour.h>
#include <algorithm>
#include <cstdio>
#include <fmt/core.h>

namespace msi::cvrp {

Tour::Tour(Graph &graph,
           Params &params,
           util::IRandomGenerator &rand)
    : m_rand(rand),
      m_params(params),
      m_passes(params.pass_count),
      m_target(params.depot_id),
      m_graph(graph),
      m_ant_completed(m_params.vehicle_count, 0),
      m_unvisited_verts(graph.vertex_count(), true) {
   m_vehicles.reserve(params.vehicle_count);
   std::generate_n(std::back_inserter(m_vehicles), params.vehicle_count, [this, &rand, count = graph.vertex_count()]() {
      return Vehicle(rand, m_params, count);
   });
}

void Tour::reset_vehicles() noexcept {
   std::for_each(m_vehicles.begin(), m_vehicles.end(), [](Vehicle &vehicle) {
      vehicle.reset();
   });
   std::fill(m_ant_completed.begin(), m_ant_completed.end(), 0);
}

void Tour::run() noexcept {
   this->reset_vehicles();
   VertexId vert_id;

   for (std::size_t vehicle_id = 0; vehicle_id < m_params.vehicle_count; vehicle_id++) {
      std::fill(m_unvisited_verts.begin(), m_unvisited_verts.end(), true);
      while (this->m_ant_completed[vehicle_id] < m_params.tour_count) {
         std::vector<bool> feasible_vertices(m_graph.vertex_count(), false);
         for (auto it = m_unvisited_verts.begin(); it != m_unvisited_verts.end(); it++) {
            vert_id = (VertexId)(it - m_unvisited_verts.begin());
            if (*it)
               if (m_vehicles[vehicle_id].m_capacity_left > m_graph.m_vertices[vert_id].demand) {
                  feasible_vertices[vert_id] = true;
               }
         }

         VertexId selected_vert = 0;
         auto feasible_vertex_count = std::count(feasible_vertices.begin(), feasible_vertices.end(), true);

         if (feasible_vertex_count > 1) {
            selected_vert = m_vehicles[vehicle_id].choose_next(m_graph, m_rand, feasible_vertices);
            if (selected_vert != m_vehicles[vehicle_id].m_current_vert) {
               m_vehicles[vehicle_id].m_capacity_left -= m_graph.m_vertices[selected_vert].demand;
               m_unvisited_verts[selected_vert] = false;
            }
         } else {
            selected_vert = m_params.depot_id;
            m_ant_completed[vehicle_id] += 1;
            m_vehicles[vehicle_id].m_capacity_left = m_params.initial_capacity;
            m_unvisited_verts[0] = false;
         }

         m_vehicles[vehicle_id].m_visited_edges.emplace_back(m_vehicles[vehicle_id].m_current_vert, selected_vert);
         m_vehicles[vehicle_id].m_visited.insert(selected_vert);
         m_vehicles[vehicle_id].m_current_vert = selected_vert;
      }

      this->update_pheromone();
   }

   auto dist = shortest_distance();
   if (dist < m_min_distance) {
      m_min_distance = dist;
   }
   fmt::print("{:0.0f} ", dist);
}

double Tour::shortest_distance() noexcept {
   double sd = 10e9;
   for (std::size_t antId = 0; antId < m_params.vehicle_count; antId++)
      if (sd > m_vehicles[antId].traveled_distance(m_graph))
         sd = m_vehicles[antId].traveled_distance(m_graph);
   return sd;
}

const Vehicle &Tour::best_vehicle() const noexcept {
   double smallest_distance = std::numeric_limits<double>::infinity();
   std::size_t id{};
   for (std::size_t vehicle_id = 0; vehicle_id < m_params.vehicle_count; vehicle_id++)
      if (smallest_distance > m_vehicles[vehicle_id].traveled_distance(m_graph))
         id = vehicle_id;

   return m_vehicles[id];
}

void Tour::update_pheromone() noexcept {
   m_graph.evaporate(this->current_iter);

   for (const Vehicle &ant : m_vehicles) {
      if (ant.current_vert() != m_target) {
         continue;
      }

      auto &edges = ant.edges();
      auto total_distance = std::accumulate(edges.begin(), edges.end(), 0.0, [this](double acc, const std::pair<VertexId, VertexId> &pair) {
         return acc + m_graph.distance(pair.first, pair.second);
      });

      for (const auto &pair : edges) {
         m_graph.add_pheromone(pair.first, pair.second, 1.0 / total_distance);
      }
   }
}

}// namespace msi::cvrp