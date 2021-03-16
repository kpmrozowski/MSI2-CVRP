project(MSI_CVRP)

add_library(MSI_CVRP Graph.cpp Ant.cpp Tour.cpp)
target_link_libraries(MSI_CVRP PUBLIC fmt)