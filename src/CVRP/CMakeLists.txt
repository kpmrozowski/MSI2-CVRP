project(MSI_CVRP)

add_library(MSI_CVRP Vechicle.cpp drawGL.cpp)
target_link_libraries(MSI_CVRP PUBLIC fmt glfw glad linmath)