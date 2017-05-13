cmake_minimum_required(VERSION 2.8)

project(HUFFMAN_LIB)

include_directories(${HUFFMAN_LIB_DIR})

set(SOURCES accomulator.cpp
			compression.cpp
			decompression.cpp
			stream_input.cpp
			stream_output.cpp
			tree.cpp
			word.cpp)
			
include_directories(header)		
	
set(HEADERS header/accomulator.h
			header/compression.h
			header/decompress.h
			header/stream_input.h
			header/stream_output.h
			header/tree.h
			header/word.h)
add_library(lib STATIC ${SOURCES} ${HEADERS})
