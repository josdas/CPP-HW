#pragma once
#ifndef STREAM_OUTPUT
#define STREAM_OUTPUT
#include "type.h"
#include <vector>
using std::vector;

class Stream_output {
	vector<machine_word> data_;
	int64_t cur_, end_;
public:
	Stream_output(vector<machine_word> const& data, int64_t end);
	explicit Stream_output(vector<machine_word> const& data);
	bool get_next();
	machine_word get_next_byte();
	bool end() const;
};
#endif
