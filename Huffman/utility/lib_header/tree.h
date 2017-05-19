#pragma once
#ifndef TREE
#define TREE
#include <vector>
#include <algorithm>
#include "word.h"
#include "accomulator.h"
#include "type.h"
using std::vector;
using std::sort;
using std::pair;

class Tree {
	vector<vector<int> > children;
	int root;
	node merge(node a, node b);
	void dfs(Word word, int v, vector<Word>& codes);
	size_t check_dfs(int v, vector<bool> &flags) const;
public:
	explicit Tree(vector<vector<int> > const& children);
	explicit Tree(Accomulator const& accomulator);
	vector<vector<int> > const& get_children() const;
	vector<Word> get_all_code();
	int get_root() const;
	int get_next(int v, bool x) const;
};
#endif
