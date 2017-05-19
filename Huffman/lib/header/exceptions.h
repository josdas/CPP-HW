#pragma once
#include <exception>

struct Decoder_tree_error : public std::exception {
	char const *what() const noexcept override {
		return "Failed to decode huffman tree";
	}
};