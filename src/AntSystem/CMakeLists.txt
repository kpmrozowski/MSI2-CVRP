project(MSI_AntSystem)

add_library(MSI_AntSystem Graph.cpp Ant.cpp Tour.cpp)
target_link_libraries(MSI_AntSystem PUBLIC fmt)