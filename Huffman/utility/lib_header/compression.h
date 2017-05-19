#pragma once
#ifndef COMPRESSION
#define COMPRESSION
#include "accomulator.h"
#include <vector>
#include "tree.h"
#include "stream_output.h"
#include "stream_input.h"

using std::vector;

class Compression {
	Tree tree_;
	vector<Word> codes_;
	Accomulator accomulator_;
public:
	explicit Compression(Accomulator accomulator);
	void compress_block(Stream_output& stream_output, Stream_input& answer) const;
	int64_t get_length() const;
	Tree const& get_tree() const;
};
#endif
