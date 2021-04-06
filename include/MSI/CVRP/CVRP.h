#ifndef CVRP_CVRP_H
#define CVRP_CVRP_H
#include <MSI/CVRP/Tour.h>
#include "Constants.h"
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

}// namespace msi::cvrp

#endif//CVRP_CVRP_H