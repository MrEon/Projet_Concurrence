cmake_minimum_required(VERSION 3.3)
project(Gestion_Threads C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
FIND_PACKAGE ( Threads REQUIRED )
set(SOURCE_FILES main.cpp Person.cpp Person.h)
add_executable(Gestion_Threads ${SOURCE_FILES})
TARGET_LINK_LIBRARIES ( Gestion_Threads ${CMAKE_THREAD_LIBS_INIT} )