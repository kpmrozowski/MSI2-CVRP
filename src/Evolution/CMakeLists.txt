project(MSI_Evolution)

add_library(MSI_Evolution Evolution.cpp)
target_link_libraries(MSI_Evolution PUBLIC fmt)