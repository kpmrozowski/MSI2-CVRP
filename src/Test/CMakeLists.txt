project(MSI_Test)

add_executable(MSI_Test AntSystemTest.cpp CVRPtest.cpp openglTest.cpp)
target_link_libraries(MSI_Test MSI_AntSystem MSI_CVRP gtest gtest_main)
