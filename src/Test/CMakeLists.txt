project(MSI_Test)

add_executable(MSI_Test CVRPtest.cpp EvolutionTest.cpp) #  AntSystemTest.cpp
target_link_libraries(MSI_Test MSI_AntSystem MSI_CVRP MSI_Evolution gtest gtest_main)
