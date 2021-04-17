#include <MSI/CVRP/CVRP.h>
#include <MSI/CVRP/Params.h>
#include <MSI/Util/IRandomGenerator.h>
#include <MSI/Util/ReadGraph.h>
#include <MSI/Util/drawGL.h>

namespace msi::cvrp {

static double regression(std::vector<double> const &vals, std::size_t n) {
   bool second_half_of_values = false;
   double N;
   std::vector<double> values;
   if (second_half_of_values) {
      N = vals.size() / 2;
      for (std::size_t i = 0; i < N; i++) {
         values.push_back(vals[i + N]);
      }
   } else {
      N = vals.size();
      values = vals;
   }
   std::vector<double> X(2 * n + 1);//Array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
   for (std::size_t i = 0; i < 2 * n + 1; i++) {
      X[i] = 0;
      for (std::size_t j = 0; j < N; j++) {
         X[i] += std::pow(j, i);//consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
      }
   }
   // fmt::print("\nX={}", X[0]);
   std::vector<std::vector<double>> B(n + 1, std::vector<double>(n + 2));//B is the Normal matrix(augmented) that will store the equations
   for (std::size_t i = 0; i <= n; i++)
      for (std::size_t j = 0; j <= n; j++)
         B[i][j] = X[i + j];    //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
   std::vector<double> Y(n + 1);//Array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
   for (std::size_t i = 0; i < n + 1; i++) {
      Y[i] = 0;
      for (std::size_t j = 0; j < N; j++)
         Y[i] += pow(j, i) * values[j];//consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
   }
   for (std::size_t i = 0; i <= n; i++)
      B[i][n + 1] = Y[i];             //load the values of Y as the last column of B(Normal Matrix but augmented)
   n++;                               //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the n of polynomial and for n n we get n+1 equations
   for (std::size_t i = 0; i < n; i++)//From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
      for (std::size_t k = i + 1; k < n; k++)
         if (B[i][i] < B[k][i])
            for (std::size_t j = 0; j <= n; j++) {
               double temp = B[i][j];
               B[i][j] = B[k][j];
               B[k][j] = temp;
            }
   for (std::size_t i = 0; i < n - 1; i++)//loop to perform the gauss elimination
      for (std::size_t k = i + 1; k < n; k++) {
         double t = B[k][i] / B[i][i];
         for (std::size_t j = 0; j <= n; j++)
            B[k][j] -= t * B[i][j];//make the elements below the pivot elements equal to zero or elimnate the variables
      }
   std::vector<double> a(n);       //'a' is for value of the final coefficients
   for (int i = n - 1; i >= 0; i--)//back-substitution
   {                               //x is an array whose values correspond to the values of x,y,z..
      a[i] = B[i][n];              //make the variable to be calculated equal to the rhs of the last equation
      for (std::size_t j = 0; j < n; j++)
         if (j != i)//then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
            a[i] -= B[i][j] * a[j];
      a[i] /= B[i][i];//now finally divide the rhs by the coefficient of the variable to be calculated
   }
   std::vector<double> distances(N);// regression values
   std::generate(distances.begin(), distances.end(), [&n, &a, x = 0.0]() mutable {
      double distance = 0.0;
      for (std::size_t i = 0; i < n - 1; i++)
         distance += a[i] * pow(x, static_cast<double>(i));
      x++;
      return distance;
   });
   return *(min(distances.begin(), distances.end()));
}

void CVRP::start_cvrp() noexcept {
   // srand(102);
   msi::util::Random r;
   msi::util::Reader re;
   // re.display_file();
   std::size_t rows = 51;
   std::size_t cols = 3;
   Table En51k5_VERT_COORD, En51k5_DEMANDS;
   En51k5_VERT_COORD = re.read_file("./src/CVRP/E-n51-k5_VERT_COORD.txt", rows, cols);
   cols = 2;
   En51k5_DEMANDS = re.read_file("./src/CVRP/E-n51-k5_DEMANDS.txt", rows, cols);
   Params p;
   Graph graph(rows);
   graph.import_vertices(En51k5_VERT_COORD, En51k5_DEMANDS);
   graph.compute_distances();
   for (std::size_t i = 0; i < rows; i++)
      for (std::size_t j = i + 1; j < rows; j++)
         graph.connect(i, j, Edge(1.0, Position(En51k5_VERT_COORD[1][i], En51k5_VERT_COORD[2][i], En51k5_VERT_COORD[1][j], En51k5_VERT_COORD[2][j])));

   std::vector<double> distances(p.iterations);
   Tour tour(graph, p, r);
   for (std::size_t iter_n = 0; iter_n < p.iterations; ++iter_n) {
      tour.m_current_iter = iter_n;
      tour.run(iter_n);
      distances[iter_n] = tour.shortest_distance().first;
   }
   auto it_min_dist = min(distances.begin(), distances.end());
   fmt::print("\nShortest_distance = {}", *it_min_dist);

   // graph.print();
   std::size_t REGRESSION_DEGREE = 11;
   auto reg = regression(distances, REGRESSION_DEGREE);
   fmt::print("\nreg_min={}", reg);

   msi::util::GraphElements graphElements{};
   graphElements.translate_vert_into_edges(tour);

   msi::util::Opengl opengl;
   opengl.draw(graphElements);
}

double train(std::vector<std::unique_ptr<msi::cvrp::Tour>> &tours, util::IRandomGenerator &rand, const Params &params, msi::evolution::Params &evo_params, const Graph &graph) {
   std::vector<double> distances(params.iterations);
   msi::cvrp::Tour tour(graph, params, rand);
   for (std::size_t i = 0; i < params.iterations; ++i) {
      tour.m_current_iter = i;
      tour.run(i);
      distances[i] = tour.shortest_distance().first;
   }
   tours.push_back(std::make_unique<msi::cvrp::Tour>(tour));
   std::size_t REGRESSION_DEGREE = 11;
   auto reg = regression(distances, REGRESSION_DEGREE);
   fmt::print("\nreg_min={}", reg);
   // return tour.min_distance() - evo_params.optimal_fitness / 2 - 170 + 2 * params.beta[0] + 10 * params.iterations / 2. * reg.second;
   return reg - evo_params.optimal_fitness;
}

Graph graph_from_file(const std::string &fn_coords, const std::string &fn_demands) {
   msi::util::Reader re;
   std::size_t rows = 51;
   std::size_t cols = 3;

   auto coords = re.read_file(fn_coords, rows, cols);
   cols = 2;
   auto demands = re.read_file(fn_demands, rows, cols);

   Graph graph(rows);
   graph.import_vertices(coords, demands);
   graph.compute_distances();

   for (std::size_t i = 0; i < rows; i++) {
      for (std::size_t j = i + 1; j < rows; j++) {
         graph.connect(i, j, Edge(1.0, Position(coords[1][i], coords[2][i], coords[1][j], coords[2][j])));
      }
   }

   return graph;
}

}// namespace msi::cvrp