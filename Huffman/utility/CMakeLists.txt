cmake_minimum_required(VERSION 2.8)

project(UTILITY)
include_directories(${UTILITY_DIR})
include_directories(header)
include_directories(${HUFFMAN_LIB_SOURCE_DIR}/header)

add_executable(utility
               main.cpp
               header/my_ifstream.h
               header/my_ofstream.h
               header/decode_tree.h
               header/encode_tree.h
               my_ifstream.cpp
               my_ofstream.cpp
			   decode_tree.cpp
			   encode_tree.cpp)

install(TARGETS utility DESTINATION bin)
target_link_libraries(utility lib)