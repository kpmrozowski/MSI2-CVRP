#ifndef CVRP_READGRAPH_H
#define CVRP_READGRAPH_H
#include <fstream>
#include <string_view>
#include <fmt/core.h>

namespace msi::util {

class Reader {
public:
    std::vector<std::vector<double>> read_file(std::string_view filename,int rows,int cols);
    void display_file();
};
using Table = std::vector<std::vector<double>>;
}// namespace msi::cvrp

#endif //CVRP_READGRAPH_H