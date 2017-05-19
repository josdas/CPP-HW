#include "decode_tree.h"
#include "exceptions.h"

int64_t get_int64(my_ifstream& ifstream, int len) {
	int64_t x = 0;
	for (auto i = 0; i < len; i++) {
		if(ifstream.end()) {
			throw new Decoder_tree_error();
		}
		int t = ifstream.get();
		x <<= 8;
		x += t;
	}
	return x;

}

template<typename T>
T check(T x, T l, T r) {
	if (x < l || x > r) {
		throw new Decoder_tree_error();
	}
	return x;
}

int get_int(my_ifstream& ifstream, int l = INT_MIN, int r = INT_MAX) {
	return check(static_cast<int>(get_int64(ifstream, 4)), l, r);
}

int64_t get_ll(my_ifstream& ifstream, int64_t l = INT64_MIN, int64_t r = INT64_MAX) {
	return check(get_int64(ifstream, 8), l, r);
}

vector<vector<int> > read_code(my_ifstream& ifstream, int64_t& r) {
	get_ll(ifstream);
	r = get_ll(ifstream);
	int64_t m = get_ll(ifstream, 0, 4 * SIZE_BLOCK);
	vector<vector<int> > temp(static_cast<size_t>(m), vector<int>(2));
	for (int64_t i = 0; i < m; i++) {
		for (int j = 0; j < 2; j++) {
			temp[static_cast<size_t>(i)][j] = get_int(ifstream, 0, 2 * m);
		}
	}
	return temp;
}

