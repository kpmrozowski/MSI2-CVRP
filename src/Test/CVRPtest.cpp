#include <MSI/AntSystem/Graph.h>
#include <MSI/AntSystem/Tour.h>
#include <MSI/Util/IRandomGenerator.h>
#include <gtest/gtest.h>
#include <MSI/CVRP/ReadGraph.h>

class Random : public msi::util::IRandomGenerator {
 public:
   double next_double(double max) noexcept override {
      return static_cast<double>(std::rand() % 10000) * max / 10000.0;
   }

   int next_int(int max) noexcept override {
      return std::rand() % max;
   }
};

TEST(CVRP, ReadGraph) {
    msi::cvrp::Reader re;
    re.read();
}