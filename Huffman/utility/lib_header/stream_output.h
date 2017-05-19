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
	int64_t get_end() const;
	machine_word get_machine_word(int64_t ind) const;
	bool end() const;
};
#endif
