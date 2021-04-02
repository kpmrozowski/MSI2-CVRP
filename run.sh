export PATH=cmake-install/bin:$PATH
export CMAKE_PREFIX_PATH=cmake-install:$CMAKE_PREFIX_PATH
cmake -S . -B out/build
make -C out/build
./out/build/src/Test/MSI_Test
