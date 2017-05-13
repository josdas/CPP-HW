#pragma once
#ifndef ACCOMULATOR
#define ACCOMULATOR
#include <vector>
#include "type.h"
using std::vector;

class Accomulator {
	vector<int64_t> counter_;
public:
	Accomulator();
	void add_block(vector<machine_word> const& block);
	int64_t get_count(size_t ind) const;
	void set(machine_word word, int64_t count);
};
#endif
