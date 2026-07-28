#include <bits/stdc++.h>
using namespace std;

#include "../algorithm/dijkstra.cpp"

int main() {
    const long long INF = (long long) 1e18;

    Dijkstra dijkstra(5);
    dijkstra.add_dir(0, 1, 2);
    dijkstra.add_dir(1, 2, 3);
    dijkstra.add_dir(2, 3, 4);
    dijkstra.add_dir(0, 3, 20);
    dijkstra.add_dir(4, 2, 1);

    dijkstra.run(0);
    assert(dijkstra.dist_all() == vector<long long>({0, 2, 5, 9, INF}));
    assert(dijkstra.path(3) == vector<int>({0, 1, 2, 3}));

    dijkstra.run(4);
    assert(dijkstra.dist_all() == vector<long long>({INF, INF, 1, 5, 0}));
    assert(dijkstra.path(3) == vector<int>({4, 2, 3}));
    assert(dijkstra.path(4) == vector<int>({4}));

    assert(dijkstra.path(0) == vector<int>({-1}));
    assert(dijkstra.path(1) == vector<int>({-1}));
    assert(dijkstra.prev[0] == -1);
    assert(dijkstra.prev[1] == -1);
}
