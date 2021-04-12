#include <MSI/CVRP/Tour.h>
#include <algorithm>
#include <fmt/core.h>
#include <cstdio>

namespace msi::cvrp {

Tour::Tour(Graph &graph,
           util::IRandomGenerator &rand,
           std::size_t ant_count,
           std::size_t passes,
           VertId target)
   : m_graph(graph)
   , m_rand(rand)
   , m_vechicles(ant_count)
   , m_passes(passes)
   , m_target(target)
   , m_ant_completed(std::vector<std::size_t>(g_n_ants, 0))
   , m_unvisited_verts(std::vector<bool>(graph.vert_count(),true))
{
   std::generate(m_vechicles.begin(),
                 m_vechicles.end(),
                 [&rand, count = graph.vert_count()]()
                 {
                    return std::make_unique<Vechicle>(rand, count);
                 });
}

void Tour::reset_vechicles() noexcept {
   std::generate(m_vechicles.begin(), m_vechicles.end(), [this]() {
     return std::make_unique<Vechicle>(m_rand, m_graph.vert_count());
   });
   std::fill(m_ant_completed.begin(), m_ant_completed.end(), 0);
}

void Tour::run() noexcept {
   this->reset_vechicles();
   // for(int i = 0; i < m_graph.vert_count(); i++){
   //    fmt::print("{}\t", i);
   //    fmt::print("{}\t", m_graph.m_vertices[i].x);
   //    fmt::print("{}\t", m_graph.m_vertices[i].y);
   //    fmt::print("{}\n", m_graph.m_vertices[i].demand);
   // }
   VertId vert_id;
   // for(int antId = 0; antId < g_n_ants; antId++)
   //    std::fill(m_vechicles[antId]->m_visited_edges.begin(), m_vechicles[antId]->m_visited_edges.end(), std::make_unique<Vechicle>(m_rand, m_graph.vert_count()));
   for(int antId = 0; antId < g_n_ants; antId++) {
      std::fill(m_unvisited_verts.begin(), m_unvisited_verts.end(), true);
      while(this->m_ant_completed[antId] < g_n_tours) {
         std::vector<bool> feasible_verts(m_graph.vert_count(), false);
         // std::set<VertId> feasible_verts_set{};
         for(auto it = m_unvisited_verts.begin(); it != m_unvisited_verts.end(); it++) {
            vert_id = (VertId)(it - m_unvisited_verts.begin());
            if(*it)
               if(m_vechicles[antId]->m_capacity_left > m_graph.m_vertices[vert_id].demand) {
                  // feasible_verts_set.insert((std::size_t)(vert_id));
                  feasible_verts[vert_id] = true;
                  // fmt::print("{:zu}\t", vert_id);
               }
         }
         // for(int i = 1; i < m_unvisited_verts.size(); i++)
         //    fmt::print("{:d}.{:d} ", i+1, (int)feasible_verts[i]);
         //    fmt::print("\n");
         VertId selected_vert, n_feasible_verts = 0;
         for(auto v : feasible_verts)
            if(v)
               n_feasible_verts++;
         if(n_feasible_verts > 1) {
            selected_vert = m_vechicles[antId]->choose_next(m_graph, m_rand, feasible_verts);
            /*
            // calculate transitions
            double sum = m_graph.probability_sum(m_vechicles[antId]->m_current_vert, feasible_verts_set);
            // for(auto vert : feasible_verts)
            //    sum += pow(m_graph.pheromone(m_vechicles[antId]->m_current_vert, vert), g_alpha) / pow(m_graph.distance(m_vechicles[antId]->m_current_vert, vert), g_beta);
            std::vector<double> transition_prob(m_graph.vert_count());
            for(auto it = feasible_verts.begin(); it != feasible_verts.end(); it++)
               if(*it == true)
                  transition_prob[it - feasible_verts.begin()] = m_graph.m_edges[m_vechicles[antId]->m_current_vert * m_graph.vert_count() + it - feasible_verts.begin()]->prob() / sum;
                  
                  //pow(m_graph.pheromone(m_vechicles[antId]->m_current_vert, it - feasible_verts.begin()), g_alpha) / pow(m_graph.distance(m_vechicles[antId]->m_current_vert, it - feasible_verts.begin()), g_beta) / sum;
            */
            if(selected_vert != m_vechicles[antId]->m_current_vert) {
               m_vechicles[antId]->m_capacity_left -= m_graph.m_vertices[selected_vert].demand;
               m_unvisited_verts[selected_vert] = false;
               // std::printf("a%d,t%d,n%d,c%d,cl%0.1f\n",
               //    antId,
               //    m_ant_completed[antId],
               //    n_feasible_verts,
               //    selected_vert+1,
               //    m_vechicles[antId]->m_capacity_left);
            }
         } else {
            selected_vert = g_depot_id;
            m_ant_completed[antId] += 1;
            m_vechicles[antId]->m_capacity_left = g_init_capacity;
            m_unvisited_verts[0] = false;
            // fmt::print("NEXT TOUR!\n");
         }
         m_vechicles[antId]->m_visited_edges.emplace_back(m_vechicles[antId]->m_current_vert, selected_vert);
         m_vechicles[antId]->m_visited.insert(selected_vert);
         m_vechicles[antId]->m_current_vert = selected_vert;
      }

      
      this->update_pheromone();
   }
   // fmt::print("{}\n", this->shortest_distance());   
}

double Tour::shortest_distance() noexcept {
   double sd = 10e9;
   for(std::size_t antId = 0; antId < g_n_ants; antId++)
      if(sd > m_vechicles[antId]->traveled_distance(m_graph))
         sd = m_vechicles[antId]->traveled_distance(m_graph);
   return sd;
}

Vechicle Tour::best_vechicle() noexcept {
   double sd = 10e9;
   std::size_t id = 10e3;
   for(std::size_t antId = 0; antId < g_n_ants; antId++)
      if(sd > m_vechicles[antId]->traveled_distance(m_graph))
         id = antId;
   Vechicle vech = *(m_vechicles[id]);
   
   return vech;
}

void Tour::update_pheromone() noexcept {
   m_graph.evaporate();

   for (std::unique_ptr<Vechicle> &ant : m_vechicles) {
      if (ant->current_vert() != m_target) {
         continue;
      }

      auto &edges = ant->edges();
      auto total_distance = std::accumulate(edges.begin(), edges.end(), 0.0, [this](double acc, const std::pair<VertId, VertId> &pair) {
        return acc + m_graph.distance(pair.first, pair.second);
      });

      for (const auto &pair : edges) {
         m_graph.add_pheromone(pair.first, pair.second, 1.0 / total_distance);
      }
   }
}

}