project(MSI_CVRP)

add_library(MSI_CVRP CVRP.cpp)
target_link_libraries(MSI_CVRP PUBLIC fmt)