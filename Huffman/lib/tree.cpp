#include "tree.h"

node Tree::merge(node a, node b) {
	root++;
	children[root][0] = static_cast<int>(a.second);
	children[root][1] = static_cast<int>(b.second);
	return {a.first + b.first, root};
}

void Tree::dfs(Word word, int v, vector<Word>& codes) {
	if (v < SIZE_BLOCK) {
		codes[v] = word;
		return;
	}
	for (int i = 0; i < 2; i++) {
		auto e = word;
		e.add_bit(i != 0);
		dfs(e, children[v][i], codes);
	}
}

Tree::Tree(vector<vector<int> > const& children): children(children) {
	root = SIZE_BLOCK * 2 - 2;
}

Tree::Tree(Accomulator const& accomulator) {
	children.resize(SIZE_BLOCK * 2, vector<int>(2));
	root = SIZE_BLOCK - 1;
	vector<node> alphabet(SIZE_BLOCK);
	vector<node> pairs;
	pairs.reserve(SIZE_BLOCK);
	for (size_t i = 0; i < SIZE_BLOCK; i++) {
		alphabet[i] = {accomulator.get_count(i), i};
	}
	sort(alphabet.begin(), alphabet.end());
	for (size_t k = 0, i = 0, j = 0; k < SIZE_BLOCK - 1; k++) {
		auto aa = i + 1 < alphabet.size()
			          ? alphabet[i].first + alphabet[i + 1].first : INF;
		auto ab = i < alphabet.size() && j < pairs.size()
			          ? alphabet[i].first + pairs[j].first : INF;
		auto bb = j + 1 < pairs.size()
			          ? pairs[j].first + pairs[j + 1].first : INF;
		if (aa <= bb && aa <= ab) {
			pairs.push_back(merge(alphabet[i], alphabet[i + 1]));
			i += 2;
		}
		else if (bb <= aa && bb <= ab) {
			pairs.push_back(merge(pairs[j], pairs[j + 1]));
			j += 2;
		}
		else {
			pairs.push_back(merge(alphabet[i], pairs[j]));
			i++;
			j++;
		}
	}
}

vector<vector<int> > const& Tree::get_children() const {
	return children;
}

vector<Word> Tree::get_all_code() {
	vector<Word> codes(SIZE_BLOCK);
	dfs(Word(), root, codes);
	return codes;
}

int Tree::get_root() const {
	return root;
}

int Tree::get_next(int v, bool x) const {
	return children[v][x];
}
