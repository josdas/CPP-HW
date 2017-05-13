#pragma once
#ifndef MY_IFSTREAM
#define MY_IFSTREAM
#include <fstream>
#include "type.h"
#include <vector>

struct my_ifstream {
	bool end();
	unsigned char get();
	explicit my_ifstream(char* _name);
	~my_ifstream();
	void reopen();
private:
	std::ifstream stream_;
	char* name_;
	std::vector<char> buffer_;
	int cur_index_;
	int last_;
};
#endif
