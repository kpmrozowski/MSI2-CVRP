project(MSI_Test)

add_executable(MSI_Test CVRPtest.cpp) #  AntSystemTest.cpp
target_link_libraries(MSI_Test MSI_AntSystem MSI_CVRP gtest gtest_main)
