#define _CRT_SECURE_NO_DEPRECATE
#include <vector>
#include "accomulator.h"
#include "compression.h"
#include "decompress.h"
#include <iostream>
#include "my_ofstream.h"
#include "my_ifstream.h"
#include "decode_tree.h"
#include "encode_tree.h"
#include <ctime>
#include <cstring>

int main(int argc, char* argv[]) {
	if (argc == 4) {
		char* name_from = argv[2];
		char* name_to = argv[3];

		my_ifstream from(name_from);
		my_ofstream to(name_to);

		if(from.fail() || to.fail()) {
			std::cerr << "Failed to open files\n";
			exit(1);
		}

		if (strcmp(argv[1], "-compress") == 0 || strcmp(argv[1], "-c") == 0) {
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
			if (from.fail()) {
				std::cerr << "Failed to reopen file\n";
				exit(1);
			}

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
		else if (strcmp(argv[1], "-decompress") == 0 || strcmp(argv[1], "-d") == 0) {
			std::cerr << "decompress\n";
			int64_t number_bits;
			vector<machine_word> temp;
			vector<vector<int> > temp_decompress;
			try {
				temp_decompress = read_code(from, number_bits);
			}
			catch(...) {
				std::cerr << "Failed to parse file\n";
				exit(1);
			}
			Decompression decompress(temp_decompress);
			std::cerr << "OK build\n";

			while (!from.end()) {
				temp.push_back(from.get());
				if (temp.size() == MAX_SIZE || from.end()) {
					int64_t last = std::min(number_bits, static_cast<int64_t>(temp.size()) * 8);
					Stream_output stream(temp, last);
					Stream_input value;
					decompress.decompress_block(stream, value);
					to.put(value);
					number_bits -= temp.size() * 8;
					temp.clear();
				}
			}

			std::cerr << "OK write\n";
		}
		std::cerr << clock() * 1.0 / CLOCKS_PER_SEC << '\n';
	}
	else {
		std::cerr << "Option:\n";
		std::cerr << "-compress file SOURCE into DEST\n";
		std::cerr << "-decompress file SOURCE into DEST\n";
	}
}