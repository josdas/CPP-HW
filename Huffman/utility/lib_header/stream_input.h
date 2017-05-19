#pragma once
#ifndef STREAM_INPUT
#define STREAM_INPUT
#include "type.h"
#include <vector>
#include "word.h"
using std::vector;

class Stream_input {
	vector<machine_word> data_;
	size_t cur_;
public:
	Stream_input();
	void add_word(Word const& word);
	size_t get_length() const;
	void add_byte(machine_word c);
	vector<machine_word> const& data() const;
};
#endif
