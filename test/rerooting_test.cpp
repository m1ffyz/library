#include <bits/stdc++.h>
using namespace std;

#include "../algorithm/rerooting.hpp"

struct S {
    long long distance_sum;
    int size;

    bool operator==(const S &other) const {
        return distance_sum == other.distance_sum and size == other.size;
    }
};

S op(S a, S b) {
    return {a.distance_sum + b.distance_sum, a.size + b.size};
}

S e() {
    return {0, 0};
}

S put(S value, int, int p) {
    if (p != -1) {
        value.distance_sum += value.size + 1;
    }
    value.size ++;
    return value;
}

struct ordered_state {
    vector<int> values;

    bool operator==(const ordered_state &other) const {
        return values == other.values;
    }
};

ordered_state ordered_op(ordered_state a, ordered_state b) {
    a.values.insert(a.values.end(), b.values.begin(), b.values.end());
    return a;
}

ordered_state ordered_e() {
    return {};
}

ordered_state ordered_put(ordered_state value, int v, int p) {
    value.values.push_back((v + 1) * 1000 + (p + 2));
    return value;
}

struct recursive_rerooting_reference {
    const vector<vector<int>> &adj;
    vector<ordered_state> dp;
    vector<ordered_state> ans;

    recursive_rerooting_reference(const vector<vector<int>> &graph)
        : adj(graph), dp(graph.size(), ordered_e()), ans(graph.size(), ordered_e()) {}

    void build() {
        dfs_bottom_up(0, -1);
        dfs_top_down(0, -1, ordered_e());
    }

    void dfs_bottom_up(int v, int p) {
        ordered_state result = ordered_e();
        for (int u : adj[v]) {
            if (u == p) continue;
            dfs_bottom_up(u, v);
            result = ordered_op(result, ordered_put(dp[u], u, v));
        }
        dp[v] = result;
    }

    void dfs_top_down(int v, int p, ordered_state value_from_parent) {
        vector<ordered_state> values;
        if (p != -1) {
            values.push_back(value_from_parent);
        }
        for (int u : adj[v]) {
            if (u == p) continue;
            values.push_back(ordered_put(dp[u], u, v));
        }

        ordered_state total = ordered_e();
        for (const auto &value : values) {
            total = ordered_op(total, value);
        }
        ans[v] = ordered_put(total, v, -1);

        int deg = (int) values.size();
        vector<ordered_state> left(deg + 1, ordered_e());
        vector<ordered_state> right(deg + 1, ordered_e());
        for (int i = 0; i < deg; i ++) {
            left[i + 1] = ordered_op(left[i], values[i]);
        }
        for (int i = deg - 1; i >= 0; i --) {
            right[i] = ordered_op(values[i], right[i + 1]);
        }

        int idx = 0;
        int child_idx_offset = (p != -1) ? 1 : 0;
        for (int u : adj[v]) {
            if (u == p) continue;
            int current_vec_idx = idx + child_idx_offset;
            ordered_state value_for_child =
                ordered_op(left[current_vec_idx], right[current_vec_idx + 1]);
            dfs_top_down(u, v, ordered_put(value_for_child, v, u));
            idx ++;
        }
    }
};

void test_single_vertex() {
    rerooting<S, op, e, put> tree(1);
    tree.build();
    assert(tree.query(0) == S({0, 1}));
}

void test_random_trees() {
    mt19937 rng(123456789);

    for (int iteration = 0; iteration < 300; iteration ++) {
        int n = (int) (rng() % 30) + 1;
        rerooting<S, op, e, put> tree(n);
        vector<vector<int>> graph(n);

        for (int v = 1; v < n; v ++) {
            int p = (int) (rng() % v);
            tree.add_edge(p, v);
            graph[p].push_back(v);
            graph[v].push_back(p);
        }

        tree.build();

        for (int root = 0; root < n; root ++) {
            vector<int> distance(n, -1);
            queue<int> que;
            distance[root] = 0;
            que.push(root);

            while (!que.empty()) {
                int v = que.front();
                que.pop();
                for (int u : graph[v]) {
                    if (distance[u] != -1) continue;
                    distance[u] = distance[v] + 1;
                    que.push(u);
                }
            }

            long long expected_sum =
                accumulate(distance.begin(), distance.end(), 0LL);
            assert(tree.query(root) == S({expected_sum, n}));
        }
    }
}

void test_aggregation_order() {
    const vector<pair<int, int>> edges = {
        {1, 3},
        {0, 2},
        {1, 4},
        {0, 1},
        {2, 5},
        {2, 6}
    };

    rerooting<ordered_state, ordered_op, ordered_e, ordered_put> tree(7);
    vector<vector<int>> graph(7);
    for (auto [u, v] : edges) {
        tree.add_edge(u, v);
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    tree.build();

    recursive_rerooting_reference reference(graph);
    reference.build();
    for (int v = 0; v < 7; v ++) {
        assert(tree.query(v) == reference.ans[v]);
    }
}

void test_long_path() {
    constexpr int N = 200000;
    rerooting<S, op, e, put> tree(N);
    for (int v = 1; v < N; v ++) {
        tree.add_edge(v - 1, v);
    }

    tree.build();

    const long long endpoint_sum = 1LL * N * (N - 1) / 2;
    assert(tree.query(0) == S({endpoint_sum, N}));
    assert(tree.query(N - 1) == S({endpoint_sum, N}));
}

int main() {
    test_single_vertex();
    test_random_trees();
    test_aggregation_order();
    test_long_path();
}
