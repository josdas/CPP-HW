#include "my_ifstream.h"

bool my_ifstream::end() {
	if (cur_index_ >= last_) {
		return stream_.peek() == EOF;
	}
	return false;
}

machine_word my_ifstream::get() {
	if (cur_index_ < last_) {
		return buffer_[cur_index_++];
	}
	stream_.read(buffer_.data(), BUFF_SIZE);
	last_ = static_cast<int>(stream_.gcount());
	cur_index_ = 0;
	return buffer_[cur_index_++];
}

my_ifstream::my_ifstream(char* _name):
	stream_(_name, std::ifstream::binary),
	name_(_name),
	buffer_(BUFF_SIZE),
	cur_index_(0),
	last_(0) { }

my_ifstream::~my_ifstream() {
	stream_.close();
}

void my_ifstream::reopen() {
	cur_index_ = 0;
	last_ = 0;
	stream_.close();
	stream_.open(name_, std::ifstream::binary);
}

bool my_ifstream::fail() const {
	return stream_.fail();
}
