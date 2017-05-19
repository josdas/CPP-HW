#include "stream_output.h"

Stream_output::Stream_output(vector<machine_word> const& data, int64_t end):
	data_(data),
	cur_(0),
	end_(end) { }

Stream_output::Stream_output(vector<machine_word> const& data):
	data_(data),
	cur_(0),
	end_(data.size() << 3) { }

bool Stream_output::get_next() {
	auto temp = cur_;
	cur_++;
	return ((data_[static_cast<size_t>(temp >> 3)] >> (temp & 7)) & 1) != 0;
}

machine_word Stream_output::get_machine_word(int64_t ind) const {
	return data_[ind];
}

bool Stream_output::end() const {
	return cur_ == end_;
}

int64_t Stream_output::get_end() const {
	return end_;
}