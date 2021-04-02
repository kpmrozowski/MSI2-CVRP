project(MSI_Test)

add_executable(MSI_Test AntSystemTest.cpp CVRPtest.cpp openglTest.cpp)

# Uncomment to switch between qt_opengl and gtest:
# target_link_libraries(MSI_Test MSI_AntSystem gtest MSI_OpenGL gtest_main)
target_link_libraries(MSI_Test MSI_AntSystem MSI_CVRP gtest gtest_main MSI_OpenGL)
