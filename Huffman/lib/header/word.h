#pragma once
#ifndef WORD
#define WORD
#include <vector>
#include "accomulator.h"

class Word {
	int64_t first_;
	int64_t second_;
	size_t cur_;
public:
	Word();
	void add_bit(bool s);
	size_t get_length() const;
	machine_word get_machine_word(size_t ind) const;
	bool get_bit(size_t ind) const;
};
#endif