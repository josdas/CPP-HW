#include "word.h"
#include "accomulator.h"

Word::Word() : first_(0), second_(0), cur_(0) {}

void Word::add_bit(bool s) {
	if (cur_ < 64) {
		first_ |= static_cast<int64_t>(s) << cur_;
	}
	else {
		second_ |= static_cast<int64_t>(s) << (cur_ - 64);
	}
	cur_++;
}

size_t Word::get_length() const {
	return cur_;
}

machine_word Word::get_machine_word(size_t ind) const { // get bits [ind * 8; ind * 8 + 7]
	if (ind < 8) {
		return (first_ >> (ind << 3)) & 255;
	}
	return (second_ >> ((ind - 8) << 3)) & 255;
}

bool Word::get_bit(size_t ind) const {
	if (ind < 64) {
		return (first_ & (1ll << ind)) != 0;
	}
	return (second_ & (1ll << (ind - 64))) != 0;
}
