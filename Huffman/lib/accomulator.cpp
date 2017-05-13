#include "accomulator.h"

Accomulator::Accomulator() {
	counter_.resize(SIZE_BLOCK);
}

void Accomulator::add_block(vector<machine_word> const& block) {
	for (auto v : block) {
		counter_[v]++;
	}
}

int64_t Accomulator::get_count(size_t ind) const {
	return counter_[ind];
}

void Accomulator::set(machine_word word, int64_t count) {
	counter_[word] = count;
}
