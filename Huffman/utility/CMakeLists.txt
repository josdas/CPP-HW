cmake_minimum_required(VERSION 2.8)

project(UTILITY)

include_directories(${UTILITY_DIR})

include_directories(header)	
add_executable(utility
               main.cpp
               header/my_ifstream.h
               header/my_ofstream.h
               my_ifstream.cpp
               my_ofstream.cpp)