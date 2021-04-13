project(MSI_Tool_Evo)

add_executable(MSI_Tool_Evo main.cpp)
target_link_libraries(MSI_Tool_Evo MSI_AntSystem MSI_CVRP MSI_Evolution Boost::program_options fmt)
