project(MSI_Test)

add_executable(MSI_Test AntSystemTest.cpp)
target_link_libraries(MSI_Test MSI_AntSystem gtest gtest_main)
