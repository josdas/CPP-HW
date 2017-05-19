#pragma once
#ifndef WORD
#define WORD
#include "accomulator.h"

class Word {
	int64_t first_;
	size_t cur_;
public:
	Word();
	void add_bit(bool s);
	size_t get_length() const;
	machine_word get_machine_word(size_t ind) const;
	bool get_bit(size_t ind) const;
};
#endif