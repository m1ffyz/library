#include <bits/stdc++.h>
using namespace std;

#include "../data_structure/rolling_hash.cpp"

int main() {
    string s = "ababa";
    double_rolling_hash<char> rh;
    rh.build(s);

    const auto &crh = rh;

    assert(crh.compare(0, 3, 2, 5) == 0);
    assert(crh.lcp(0, 3, 2, 5) == 3);

    assert(crh.compare(0, 2, 0, 3) == -1);
    assert(crh.compare(0, 3, 0, 2) == 1);

    assert(crh.compare(0, 3, 1, 4) == -1);
    assert(crh.compare(1, 4, 0, 3) == 1);
}
