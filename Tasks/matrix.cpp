#define _ijps 01
#define _CRT_SECURE_NO_DEPRECATE
//#pragma comment(linker, "/STACK:667772160")
#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <map>
#include <set>
#include <deque>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <algorithm>
#include <string>
#include <fstream>
#include <assert.h> 
#include <list>
#include <cstring>
#include <queue>
using namespace std;

#define name "absmarkchain"
#define times clock() * 1.0 / CLOCKS_PER_SEC

typedef unsigned int uint;
typedef unsigned long long ull;
typedef long long ll;


struct __isoff {
    __isoff() {
        if(_ijps)
            freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);//, freopen("test.txt", "w", stderr);
        else freopen(name".in", "r", stdin), freopen(name".out", "w", stdout);
        //ios_bsume::sync_with_stdio(0);
        //srand(time(0));
        srand('C' + 'T' + 'A' + 'C' + 'Y' + 'M' + 'B' + 'A');
    }
    ~__isoff() {
        //if(_ijps) cout<<times<<'\n';
    }
} __osafwf;
const ull p1 = 131;
const ull p2 = 129;
const double eps = 1e-8;
const double pi = acos(-1.0);

const int infi = 1e9 + 7;
const ll inf = 1e18 + 7;
const ll dd = 5e5 + 7;

typedef long long ll;
#define forn(i, n) for(int i = 0; i < n; i++)
#define fornn(i, a, n) for(int i = a; i < n; i++)
#define times clock() * 1.0 / CLOCKS_PER_SEC

template<typename E>
struct matrix {
    static matrix get_I(int n) {
        matrix result(n);
        for(int i = 0; i < n; i++) {
            result.get(i, i) = 1;
        }
        return result;
    }

    matrix() { }

    matrix(matrix const& T) : data(T.data) { }

    matrix(size_t n, size_t m) {
        data.resize(n, vector<E>(m));
    }

    matrix(size_t n) : matrix(n, n) { }

    size_t get_height() const {
        return data.size();
    }

    size_t get_width() const {
        if(data.size() == 0) {
            return 0;
        }
        return data[0].size();
    }

    E& get(size_t x, size_t y) {
        return data[x][y];
    }

    E get(size_t x, size_t y) const {
        return data[x][y];
    }

    vector<E>& get_vector(size_t x) {
        return data[x];
    }

    vector<E> get_vector(size_t x) const {
        return data[x];
    }

    template<typename T>
    friend matrix<T> operator*(matrix<T> const& first, matrix<T> const& second);

    template<typename T>
    friend matrix<T> operator-(matrix<T> const& first, matrix<T> const& second);

    template<typename T>
    friend matrix<T> transpose(matrix<T> const& a);

    template<typename T>
    friend matrix<T> inverse(matrix<T> a);

private:
    vector<vector<E> > data;
};

template<typename T>
matrix<T> transpose(matrix<T> const& a) {
    matrix<T> result(a.get_width(), a.get_height());
    for(size_t i = 0; i < a.get_height(); i++) {
        for(size_t j = 0; j < a.get_width(); j++) {
            result.get(j, i) = a.get(i, j);
        }
    }
    return result;
}

template<typename T>
matrix<T> operator*(matrix<T> const& first, matrix<T> const& second) {
    assert(first.get_width() == second.get_height());
    matrix<T> result(first.get_height(), second.get_width());
    matrix<T> tmpSecond = transpose(second);
    for(size_t i = 0; i < result.get_height(); i++) {
        for(size_t j = 0; j < result.get_width(); j++) {
            T& ans = result.get(i, j);
            for(size_t k = 0; k < first.get_width(); k++) {
                ans += first.get(i, k) * tmpSecond.get(j, k);
            }
        }
    }
    return result;
}

template<typename T>
matrix<T> operator-(matrix<T> const& first, matrix<T> const& second) {
    assert(first.get_height() == second.get_height()
        && first.get_width() == second.get_width());
    matrix<T> result(first);
    for(size_t i = 0; i < first.get_height(); i++) {
        for(size_t j = 0; j < first.get_width(); j++) {
            result.get(i, j) -= second.get(i, j);
        }
    }
    return result;
}

template<typename T>
matrix<T> inverse(matrix<T> a) {
    assert(a.get_height() == a.get_width());
    size_t n = a.get_height();
    matrix<T> inv = matrix<T>::get_I(n);
    for(size_t i = 0; i < n; i++) {
        for(size_t j = i; j < n; j++) {
            if(abs(a.get(j, i)) > eps) {
                swap(a.get_vector(i), a.get_vector(j));
                swap(inv.get_vector(i), inv.get_vector(j));
                break;
            }
        }
        double temp = a.get(i, i);
        for(size_t j = 0; j < n; j++) {
            a.get(i, j) /= temp;
            inv.get(i, j) /= temp;
        }
        for(size_t j = 0; j < n; j++) {
            if(j != i) {
                temp = a.get(j, i);
                for(size_t k = 0; k < n; k++) {
                    a.get(j, k) -= temp * a.get(i, k);
                    inv.get(j, k) -= temp * inv.get(i, k);
                }
            }
        }
    }
    return inv;
}

struct edge {
    int a, b;
    double f;

    edge() { };

    edge(int _a, int _b, double _f) : a(_a), b(_b), f(_f) { };

    friend istream& operator>>(istream& ins, edge& e) {
        ins >> e.a >> e.b >> e.f;
        e.a--;
        e.b--;
        return ins;
    }
    bool is_absorbing() {
        return a == b && 1 - f < eps;
    }
};

int main() {
    size_t n, m;
    cin >> n >> m;
    vector<edge> edges(m);
    vector<bool> absorbing(n);
    vector<int> pos(n);
    for(size_t i = 0; i < m; i++) {
        int a, b;
        double f;
        scanf("%d%d%lf", &a, &b, &f);
        edges[i] = edge(a - 1, b - 1, f);
        if(edges[i].is_absorbing()) {
            absorbing[edges[i].a] = true;
        }
    }
    size_t count_q = 0, count_r = 0;
    for(size_t i = 0; i < n; i++) {
        if(!absorbing[i]) {
            pos[i] = count_q++;
        }
        else {
            pos[i] = count_r++;
        }
    }
    matrix<double> Q(count_q);
    matrix<double> R(count_q, count_r);
    for(size_t i = 0; i < m; i++) {
        int a = edges[i].a;
        int b = edges[i].b;
        if(!absorbing[a] && absorbing[b]) {
            R.get(pos[a], pos[b]) = edges[i].f;
        }
        if(!absorbing[a] && !absorbing[b]) {
            Q.get(pos[a], pos[b]) = edges[i].f;
        }
    }
    matrix<double> E = matrix<double>::get_I(count_q) - Q;
    matrix<double> N = inverse(E);
    matrix<double> G = N * R;
    for(size_t i = 0; i < n; i++) {
        double ans = 0;
        if(absorbing[i]) {
            for(size_t j = 0; j < n; j++) {
                if(!absorbing[j]) {
                    ans += G.get(pos[j], pos[i]);
                }
            }
            ans = (ans + 1) / n;
        }
        printf("%0.15f\n", ans);
    }
}