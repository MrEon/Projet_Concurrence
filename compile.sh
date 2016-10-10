cmake .
mkdir -p bin
cp Makefile src/Makefile
cp -r CMakeFiles src/CMakeFiles
cp CMakeCache.txt src/CMakeCache.txt
cp cmake_install.cmake src/cmake_install.cmake 
cd src
make
mv Projet_Concurrence ../bin/Projet_Concurrence
rm -r CMakeFiles
rm CMakeCache.txt
rm cmake_install.cmake
rm Makefile
