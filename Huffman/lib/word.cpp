#include "word.h"
#include "accomulator.h"

Word::Word() : 
	first_(0),
	cur_(0) {}

void Word::add_bit(bool s) {
	first_ |= static_cast<int64_t>(s) << cur_;
	cur_++;
}

size_t Word::get_length() const {
	return cur_;
}

machine_word Word::get_machine_word(size_t ind) const { // get bits [ind * 8; ind * 8 + 7]
	return (first_ >> (ind << 3)) & 255;
}

bool Word::get_bit(size_t ind) const {
	return (first_ & (1ll << ind)) != 0;
}
