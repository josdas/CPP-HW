cmake_minimum_required(VERSION 2.8)

project(HUFFMAN_LIB)
include_directories(${HUFFMAN_LIB_DIR})
include_directories(header)

set(SOURCES accomulator.cpp
			compression.cpp
			decompression.cpp
			stream_input.cpp
			stream_output.cpp
			tree.cpp
			word.cpp)
			
set(TEST	huffman_testing
			huffman_testing.cpp
			gtest/gtest-all.cc
			gtest/gtest.h
			gtest/gtest_main.cc)		
	
set(HEADERS header/accomulator.h
			header/compression.h
			header/decompress.h
			header/stream_input.h
			header/stream_output.h
			header/tree.h
			header/word.h
			header/exceptions.h)
			
add_library(lib ${SOURCES} ${HEADERS})
		
install (TARGETS lib DESTINATION lib)

install (FILES ${HEADERS} DESTINATION include/lib)

add_executable(testing ${SOURCES} ${HEADERS} ${TEST})

target_link_libraries(testing -lpthread)
