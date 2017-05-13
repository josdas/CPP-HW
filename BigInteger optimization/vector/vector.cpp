#define _ijps 01
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <cmath>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "vector.h"
using namespace std;

#define name ""
#define times clock() * 1.0 / CLOCKS_PER_SEC

struct __isoff {
	__isoff() {
		if (_ijps)
			freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
		srand('C' + 'T' + 'A' + 'C' + 'Y' + 'M' + 'B' + 'A');
	}
	~__isoff() {
	}
} __osafwf;

vector<int> E;

int main() {
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	for(int i = 0; i < 10; i++) {
		E.push_back(i);
	}
	vector<int> W(E);
	for (size_t i = 0; i < 10; i++) {
		W.push_back(i * 3);
	}

	for (size_t i = 0; i < W.size(); i++) {
		cerr << W[i] << ' ';
	}
	cerr << '\n';	
	W.insert(W.begin(), -1);
	for (size_t i = 0; i < W.size(); i++) {
		cerr << W[i] << ' ';
	}
	cerr << '\n';
	W.erase(W.begin());
	for (size_t i = 0; i < W.size(); i++) {
		cerr << W[i] << ' ';
	}
	cerr << '\n';
	W.insert(W.begin(), W[1]);
	for (size_t i = 0; i < W.size(); i++) {
		cerr << W[i] << ' ';
	}
	cerr << '\n';
	W.erase(W.begin(), W.end());
	for(size_t i = 0; i < W.size(); i++) {
		cerr << W[i] << ' ';
	}

	for (size_t i = 0; i < 10; i++) {
		W.push_back(i * 3);
	}
	for (size_t i = 0; i < W.size(); i++) {
		cerr << W[i] << ' ';
	}
	cerr << '\n';

	W.clear();
	for (size_t i = 0; i < W.size(); i++) {
		cerr << W[i] << ' ';
	}
	cerr << '\n';


	W.push_back(1);
	for(size_t i = 0; i < 100; i++) {
		W.push_back(W[i]);
	}
	for (size_t i = 0; i < W.size(); i++) {
		cerr << W[i] << ' ';
	}
	cerr << '\n';
}
