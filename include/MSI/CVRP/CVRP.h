#ifndef CVRP_CVRP_H
#define CVRP_CVRP_H
#include <MSI/CVRP/Tour.h>
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <vector>

namespace msi::cvrp {

class CVRP {
public:
  void start_cvrp() noexcept;
};

double train(std::vector<msi::cvrp::Tour> &tours, util::IRandomGenerator &rand, Params &params, Graph &graph);
Graph graph_from_file(Params &params, const std::string &fn_coords, const std::string &fn_demands);

}// namespace msi::cvrp

#endif//CVRP_CVRP_H