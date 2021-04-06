#include <MSI/Util/IRandomGenerator.h>
#include <MSI/Util/drawGL.h>
#include <MSI/Util/ReadGraph.h>
#include <MSI/CVRP/CVRP.h>

namespace msi::cvrp {

void CVRP::start_cvrp() noexcept {
    msi::util::Random r;
    msi::util::Reader re;
    // re.display_file();
    std::size_t rows = 51;
	std::size_t cols = 3;
	Table En51k5_VERT_COORD, En51k5_DEMANDS;
	En51k5_VERT_COORD = re.read_file("./src/CVRP/E-n51-k5_VERT_COORD.txt",rows,cols);
    cols = 2;
	En51k5_DEMANDS = re.read_file("./src/CVRP/E-n51-k5_DEMANDS.txt",rows,cols);
    Graph graph(rows);
    graph.import_vertices(En51k5_VERT_COORD, En51k5_DEMANDS);
    graph.compute_distances();
    for(std::size_t i = 0; i < rows; i++)
        for(std::size_t j = i + 1; j < rows; j++)
            graph.connect(i, j, Edge(1.0, Position(En51k5_VERT_COORD[1][i], En51k5_VERT_COORD[2][i], En51k5_VERT_COORD[1][j], En51k5_VERT_COORD[2][j])));

   srand(111);
   Tour tour(graph, r);//, 100, 50, 0);
   for(std::size_t iter_n = 0; iter_n < g_iterations; ++iter_n) {
       tour.run();
   }

   // graph.print();
   
   msi::util::GraphElements graphElements;
   graphElements.translate_vert_into_edges(tour);

   msi::util::Opengl opengl;
   opengl.draw(graphElements);
}


}// namespace msi::cvrp