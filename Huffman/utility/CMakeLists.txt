cmake_minimum_required(VERSION 2.8)

project(UTILITY)
include_directories(${UTILITY_DIR})
include_directories(header)
include_directories(lib_header)

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
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(utility -lib)
else ()
	target_link_libraries(utility lib.lib)
endif()