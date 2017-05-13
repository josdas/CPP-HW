#pragma once
#ifndef DECOMPRESSION
#define DECOMPRESSION
#include "tree.h"
#include "stream_output.h"
#include "stream_input.h"

using std::vector;

class Decompression {
	Tree tree_;
	int cur_vertex;
public:
	explicit Decompression(vector<vector<int> > const& temp);
	void decompress_block(Stream_output& stream, Stream_input& answer);
};
#endif
