#include "compression.h"
#include "stream_input.h"
#include "stream_output.h"

Compression::Compression(Accomulator accomulator): 
	tree_(accomulator),
	accomulator_(accomulator) {
	codes_ = tree_.get_all_code();
}

void Compression::compress_block(Stream_output& stream_output, Stream_input& stream) const {
	int64_t end = stream_output.get_end() >> 3;
	for (int64_t i = 0; i < end; i++) {
		auto word = stream_output.get_machine_word(i);
		stream.add_word(codes_[word]);
	}
}

int64_t Compression::get_length() const {
	int64_t size = 0;
	for (int i = 0; i < SIZE_BLOCK; i++) {
		size += 1ll * accomulator_.get_count(i) * codes_[i].get_length();
	}
	return size;
}

Tree const& Compression::get_tree() const {
	return tree_;
}
