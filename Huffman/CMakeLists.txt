cmake_minimum_required(VERSION 2.8)

project(HUFFMAN_TESTING)

include_directories(${HUFFMAN_TESTING_DIR})

add_executable(huffman_testing
               huffman_testing.cpp
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc)
			   
include_directories(lib)			   
add_subdirectory(lib)	
include_directories(utility)			   
add_subdirectory(utility)				   
include_directories(lib/header)		
target_link_libraries(huffman_testing lib -lgmp -lgmpxx -lpthread)