#ifndef CVRP_READGRAPH_H
#define CVRP_READGRAPH_H
#include <iostream>
#include <fstream>
#include <string>
#include <fmt/core.h>

namespace msi::cvrp {

class Reader {
public:
    double** read_file(std::string filename,int rows,int cols);
    void display_file();
};

}// namespace msi::cvrp

#endif //CVRP_READGRAPH_H