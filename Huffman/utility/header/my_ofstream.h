#pragma once
#ifndef MY_OFSTREAM
#define MY_OFSTREAM
#include <fstream>
#include "stream_input.h"

struct my_ofstream {
	explicit my_ofstream(char* _name);
	void put(machine_word v);
	void put(Stream_input const& stream);
	void finalize();
	~my_ofstream();

private:
	std::ofstream stream_;
	char* name_;
	char remainder_;
	int shift_;
	vector<char> buffer_;
	int cur_index_;
};
#endif
