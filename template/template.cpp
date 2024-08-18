#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
#define rep(i, n) for(int i = 0; i < (int) (n); i ++)
#define rrep(i, m, n) for(int i = (int) m; i < (int) n; i ++)
#define forin(x, a) for(auto &x : a)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};
const int ddx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int ddy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

long long gcd(long long a, long long b){
    if(a % b == 0) return b;
    else return gcd(b, a % b);
}

long long lcm(long long a, long long b){
    return a * b / gcd(a, b);
}

template<typename T>
void v_cin(vector<T> &x){
    rep(i, x.size()) cin >> x[i];
}

template<typename T>
void vv_cin(vector<vector<T>> &x){
    rep(i, x.size()) rep(j, x[i].size()) cin >> x[i][j];
}

template <typename T>
void v_cout(vector<T> &x){
    rep(i, x.size()) cout << x[i] << (i < x.size() - 1 ? " " : "\n");
}

template <typename T>
void vv_cout(vector<vector<T>> &x){
    rep(i, x.size()) v_cout(x[i]);
}
