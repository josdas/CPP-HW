#define _CRT_SECURE_NO_DEPRECATE
#include <vector>
#include "accomulator.h"
#include "compression.h"
#include "decompress.h"
#include <iostream>
#include "my_ofstream.h"
#include "my_ifstream.h"
#include <ctime>
#include <cstring>

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

int64_t get_int64(my_ifstream& ifstream, int len) {
	int64_t x = 0;
	for (auto i = 0; i < len; i++) {
		int t = ifstream.get();
		x <<= 8;
		x += t;
	}
	return x;
	
}

int get_int(my_ifstream& ifstream) {
	return static_cast<int>(get_int64(ifstream, 4));
}

int64_t get_ll(my_ifstream& ifstream) {
	return get_int64(ifstream, 8);
}

Decompression read_code(my_ifstream& ifstream, int64_t& r) {
	get_ll(ifstream);
	r = get_ll(ifstream);
	int64_t m = get_ll(ifstream);
	vector<vector<int> > temp(static_cast<size_t>(m), vector<int>(2));
	for (int64_t i = 0; i < m; i++) {
		for (int j = 0; j < 2; j++) {
			temp[static_cast<size_t>(i)][j] = get_int(ifstream);
		}
	}
	return Decompression(temp);
}

int main(int argc, char* argv[]) {
	if (argc == 4) {
		char* name_from = argv[2];
		char* name_to = argv[3];
		my_ifstream from(name_from);
		my_ofstream to(name_to);
		std::cerr << "GO ";
		if (strcmp(argv[1], "compress") == 0) {
			std::cerr << "compress\n";
			Accomulator accomulator;
			vector<machine_word> temp;
			while (!from.end()) {
				temp.push_back(from.get());
				if (temp.size() == MAX_SIZE || from.end()) {
					accomulator.add_block(temp);
					temp.clear();
				}
			}
			std::cerr << "OK accomulator\n";
			Compression compression(accomulator);
			from.reopen();
			write_code(compression, to);
			std::cerr << "OK build tree\n";
			while (!from.end()) {
				temp.push_back(from.get());
				if (temp.size() == MAX_SIZE || from.end()) {
					Stream_output stream(temp);
					Stream_input value;
					compression.compress_block(stream, value);
					to.put(value);
					temp.clear();
				}
			}
			std::cerr << "OK write\n";
		}
		else if (strcmp(argv[1], "decompress") == 0) {
			std::cerr << "decompress\n";
			int64_t right;
			auto decompress = read_code(from, right);
			std::cerr << "OK build\n";
			vector<machine_word> temp;
			while (!from.end()) {
				temp.push_back(from.get());
				if (temp.size() == MAX_SIZE || from.end()) {
					int64_t last = std::min(right, static_cast<int64_t>(temp.size()) * 8);
					Stream_output stream(temp, last);
					Stream_input value;
					decompress.decompress_block(stream, value);
					to.put(value);
					right -= temp.size() * 8;
					temp.clear();
				}
			}
		}
		std::cerr << clock() * 1.0 / CLOCKS_PER_SEC << '\n';
	}
}