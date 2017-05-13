#include <algorithm>
#include <cassert>
#include <vector>
#include <utility>
#include "gtest/gtest.h"
#include "decompress.h"
#include "compression.h"

void CHECK(Accomulator accomulator, vector<machine_word> T) {
	Compression compression(accomulator);
	Stream_input stream_input;
	Stream_output stream_output(T);
	compression.compress_block(stream_output, stream_input);

	Decompression decompression(compression.get_tree().get_children());
	Stream_output stream_output_d(stream_input.data(), stream_input.get_length());
	Stream_input stream_input_d;
	decompression.decompress_block(stream_output_d, stream_input_d);

	if (T != stream_input_d.data()) {
		for (int i = 0; i < T.size(); i++) {
			if (T[i] != stream_input_d.data()[i]) {
				std::cerr << 2;
			}
		}
		std::cerr << 1;
	}
	ASSERT_EQ(T, stream_input_d.data());
}

TEST(correctness, random_tests) {
	int size = 1;
	for (int i = 0; i < 300; i++) {
		vector<machine_word> T(size);
		for (int j = 0; j < size; j++) {
			T[j] = rand() % 256;
		}
		Accomulator accomulator;
		accomulator.add_block(T);
		CHECK(accomulator, T);
		size = rand() % (1000 * 100);
	}
}

TEST(correctness, random_tests_several_part) {
	int size = 1;
	for (int i = 0; i < 100; i++) {
		vector<machine_word> T(size);
		for (int j = 0; j < size; j++) {
			T[j] = rand() % 256;
		}
		Accomulator accomulator;
		accomulator.add_block(T);
		Compression compression(accomulator);
		Stream_input stream_input;
		Stream_output stream_output(T);
		compression.compress_block(stream_output, stream_input);

		Decompression decompression(compression.get_tree().get_children());
		Stream_input stream_input_d;
		auto data = stream_input.data();
		vector<machine_word> temp;

		int block_size = rand() % 1000 + 1;
		auto threshold = stream_input.get_length();
		for (size_t j = 0; j < data.size(); j++) {
			temp.push_back(data[j]);
			if (temp.size() > block_size || j + 1 == data.size()) {
				size_t r = std::min(threshold, temp.size() * 8);
				threshold -= r;
				Stream_output stream_output_d(temp, r);
				decompression.decompress_block(stream_output_d, stream_input_d);
				temp.clear();
			}
		}
		ASSERT_EQ(T, stream_input_d.data());
		size = rand() % (5000 * 10);
	}
}

TEST(correctness, random_big_accomulator) {
	int size = 1;
	Accomulator accomulator;
	for (int i = 0; i < 100; i++) {
		vector<machine_word> T(size);
		for (int j = 0; j < size; j++) {
			T[j] = rand() % 256;
		}
		accomulator.add_block(T);
		CHECK(accomulator, T);
		size = std::min(size * 5 / 4 + 1, 1000 * 100);
	}
}


TEST(correctness, same_byte) {
	int size = 1;
	Accomulator accomulator;
	for (int i = 0; i < 100; i++) {
		vector<machine_word> T(size);
		accomulator.add_block(T);
		CHECK(accomulator, T);
		size = std::min(size * 5 / 4 + 1, 1000 * 100);
	}
}


TEST(correctness, all_different) {
	int size = 1;
	Accomulator accomulator;
	for (int i = 0; i < 100; i++) {
		vector<machine_word> T(size);
		for (int j = 0; j < size; j++) {
			T[j] = j % 256;
		}
		accomulator.add_block(T);
		CHECK(accomulator, T);
		size = std::min(size * 5 / 4 + 1, 1000 * 100);
	}
}


TEST(correctness, all_binary) {
	int size = 100 * 100;
	Accomulator accomulator;
	for (int i = 0; i < 25; i++) {
		vector<machine_word> T(1 << i, i);
		accomulator.add_block(T);
	}
	for (int i = 0; i < 100; i++) {
		vector<machine_word> T(size);
		for (int j = 0; j < size; j++) {
			T[j] = rand() % 256;
		}
		CHECK(accomulator, T);
	}
}

TEST(correctness, big_all_binary_1) {
	int size = 1000 * 100;
	Accomulator accomulator;
	for (int64_t i = 0; i < 60; i++) {
		accomulator.set(static_cast<machine_word>(i), static_cast<int64_t>(1) << i);
	}
	for (int i = 0; i < 50; i++) {
		vector<machine_word> T(size);
		for (int j = 0; j < size; j++) {
			T[j] = rand() % 256;
		}
		CHECK(accomulator, T);
	}
}

TEST(correctness, big_all_binary_2) {
	int size = 1000 * 100;
	Accomulator accomulator;
	for (int64_t i = 0; i < 60; i++) {
		accomulator.set(static_cast<machine_word>(i), static_cast<int64_t>(1) << std::min(i, static_cast<int64_t>(60)));
	}
	for (int i = 0; i < 50; i++) {
		vector<machine_word> T(size);
		for (int j = 0; j < size; j++) {
			T[j] = rand() % 256;
		}
		CHECK(accomulator, T);
	}
}
