#include "encode_tree.h"
#include <vector>

void add_word_to_vector(vector<char>& temp, vector<char> const& word) {
	for (auto v : word) {
		temp.push_back(v);
	}
}

vector<char> to_char(int x) {
	vector<char> temp(4);
	for (auto i = 0; i < 4; i++) {
		temp[3 - i] = x & ((1 << 8) - 1);
		x >>= 8;
	}
	return temp;
}

vector<char> to_char_ll(int64_t x) {
	vector<char> temp(8);
	for (auto i = 0; i < 8; i++) {
		temp[7 - i] = x & ((1ll << 8) - 1);
		x >>= 8ll;
	}
	return temp;
}

vector<char> to_vector(Compression const& compression) {
	vector<char> result;
	auto temp = compression.get_tree().get_children();
	add_word_to_vector(result, to_char_ll(4 * (temp.size() * 2) + 3 * 8));
	add_word_to_vector(result, to_char_ll(compression.get_length()));
	add_word_to_vector(result, to_char_ll(temp.size()));
	for (size_t i = 0; i < temp.size(); i++) {
		for (int j = 0; j < 2; j++) {
			add_word_to_vector(result, to_char(temp[i][j]));
		}
	}
	return result;
}

void write_code(Compression const& compression, my_ofstream& ofstream) {
	auto temp = to_vector(compression);
	for (auto v : temp) {
		ofstream.put(v);
	}
}
