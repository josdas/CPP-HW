#include "stream_input.h"
#include <iostream>
Stream_input::Stream_input(): cur_(0) { }

void Stream_input::add_word(Word const& word) {
	data_.resize((cur_ + word.get_length() + 8) >> 3);
	machine_word remainder = data_[cur_ >> 3];
	size_t shift = cur_ & 7;
	size_t l = word.get_length() & 7;
	size_t i;
	for (i = 0; (i + 1) * 8 <= word.get_length(); i++ , cur_ += 8) {
		auto temp = word.get_machine_word(i);
		data_[cur_ >> 3] = remainder | (temp << shift);
		remainder = temp >> (8 - shift);
	}
	auto temp = word.get_machine_word(i);
	data_[cur_ >> 3] = remainder | (temp << shift);
	cur_ += l;
	if (shift + l >= 8) {
		data_[cur_ >> 3] = temp >> (8 - shift);
	}
}

size_t Stream_input::get_length() const {
	return cur_;
}

void Stream_input::add_byte(machine_word c) {
	data_.push_back(c);
	cur_ += 8;
}

vector<machine_word> const& Stream_input::data() const {
	return data_;
}
