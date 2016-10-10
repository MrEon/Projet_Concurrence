mkdir -p bin
cd src
cmake .
make
mv Projet_Concurrence ../bin/Projet_Concurrence
rm -r CMakeFiles
rm CMakeCache.txt
rm cmake_install.cmake
rm Makefile
