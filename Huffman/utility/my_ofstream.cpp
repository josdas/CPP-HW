#include "my_ofstream.h"

my_ofstream::my_ofstream(char* _name):
	stream_(_name, std::ofstream::binary),
	name_(_name),
	remainder_(0),
	shift_(0),
	buffer_(BUFF_SIZE),
	cur_index_(0) { }

void my_ofstream::put(machine_word v) {
	if (cur_index_ < BUFF_SIZE) {
		buffer_[cur_index_++] = v;
		return;
	}
	stream_.write(buffer_.data(), BUFF_SIZE);
	cur_index_ = 0;
	buffer_[cur_index_++] = v;
}

void my_ofstream::put(Stream_input const& stream) {
	auto buffer = stream.data();
	int64_t l = stream.get_length();
	for (size_t i = 0; i + 1 < buffer.size(); i++, l -= 8) {
		machine_word v = buffer[i];
		machine_word temp = remainder_ | (v << shift_);
		put(temp);
		remainder_ = v >> shift_;
		if (shift_ == 0) {
			remainder_ = 0;
		}
	}
	if (shift_ + l >= 8) {
		put(remainder_ | (buffer.back() << shift_));
		remainder_ = buffer.back() >> shift_;
	}
	else {
		remainder_ = remainder_ | (buffer.back() << shift_);
	}
	shift_ = (shift_ + l) & 7;
	if (shift_ == 0) {
		remainder_ = 0;
	}
}

void my_ofstream::finalize() {
	if (shift_ > 0) {
		put(remainder_);
		shift_ = 0;
	}
	stream_.write(buffer_.data(), cur_index_);
}

my_ofstream::~my_ofstream() {
	finalize();
	stream_.close();
}

bool my_ofstream::fail() const {
	return stream_.fail();
}
