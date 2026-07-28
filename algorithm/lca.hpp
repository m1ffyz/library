/* @file lca.hpp
 * @brief LCA
 * @author m1ffyz
 * @date 2026-06-26
 * @see https://github.com/m1ffyz/library/blob/main/algorithm/lca.hpp
 */

// using S = ...;
// S op(S a, S b);
// S e();
//
// op は可換であることを仮定する。
// 例: 和, max, min, xor, gcd など。

template<class S, S (*op)(S, S), S (*e)()>
struct LCA {
    struct Edge {
        int to;
        S val;
    };

    int n;
    int LOG;

    vector<vector<Edge>> g;

    vector<vector<int>> par;
    vector<vector<S>> acc;

    vector<int> dep;
    vector<int> tin, tout, sub_size;
    vector<int> ord;
    vector<vector<int>> child;

    LCA(int n_) : n(n_) {
        LOG = 1;
        while ((1LL << LOG) <= max(1, n)) LOG++;

        g.assign(n, {});
        par.assign(LOG, vector<int>(n, -1));
        acc.assign(LOG, vector<S>(n, e()));

        dep.assign(n, -1);
        tin.assign(n, -1);
        tout.assign(n, -1);
        sub_size.assign(n, 0);
        child.assign(n, {});
    }

    // 無向木の辺を追加する。
    // val はその辺に乗せる可換モノイドの値。
    void add_edge(int u, int v, S val = e()) {
        g[u].push_back({v, val});
        g[v].push_back({u, val});
    }

    void build(int root = 0) {
        for (int k = 0; k < LOG; k++) {
            fill(par[k].begin(), par[k].end(), -1);
            fill(acc[k].begin(), acc[k].end(), e());
        }

        fill(dep.begin(), dep.end(), -1);
        fill(tin.begin(), tin.end(), -1);
        fill(tout.begin(), tout.end(), -1);
        fill(sub_size.begin(), sub_size.end(), 0);

        ord.clear();
        child.assign(n, {});

        vector<int> st;
        st.push_back(root);

        dep[root] = 0;
        par[0][root] = -1;
        acc[0][root] = e();

        while (!st.empty()) {
            int v = st.back();
            st.pop_back();

            tin[v] = (int)ord.size();
            ord.push_back(v);

            for (auto ed : g[v]) {
                int u = ed.to;
                if (dep[u] != -1) continue;

                dep[u] = dep[v] + 1;
                par[0][u] = v;
                acc[0][u] = ed.val;

                child[v].push_back(u);
                st.push_back(u);
            }
        }

        for (int v : ord) {
            sub_size[v] = 1;
        }

        for (int i = (int)ord.size() - 1; i >= 0; i--) {
            int v = ord[i];

            for (int c : child[v]) {
                sub_size[v] += sub_size[c];
            }

            tout[v] = tin[v] + sub_size[v];
        }

        for (int k = 0; k + 1 < LOG; k++) {
            for (int v = 0; v < n; v++) {
                int p = par[k][v];

                if (p == -1) {
                    par[k + 1][v] = -1;
                    acc[k + 1][v] = acc[k][v];
                } else {
                    par[k + 1][v] = par[k][p];
                    acc[k + 1][v] = op(acc[k][v], acc[k][p]);
                }
            }
        }
    }

    // v から k 個上の祖先。
    // 存在しない場合は -1。
    int climb(int v, long long k) const {
        if (k < 0) return -1;

        for (int i = 0; i < LOG; i++) {
            if ((k >> i) & 1LL) {
                if (v == -1) return -1;
                v = par[i][v];
            }
        }

        if ((k >> LOG) > 0) return -1;
        return v;
    }

    int lca(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);

        u = climb(u, dep[u] - dep[v]);

        if (u == v) return u;

        for (int k = LOG - 1; k >= 0; k--) {
            if (par[k][u] != par[k][v]) {
                u = par[k][u];
                v = par[k][v];
            }
        }

        return par[0][u];
    }

    int dist(int u, int v) const {
        int w = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[w];
    }

    // v から anc まで登る辺の値の積。
    // anc は v の祖先であることを仮定する。
    S fold_to_ancestor(int v, int anc) const {
        S res = e();
        int len = dep[v] - dep[anc];

        for (int k = 0; k < LOG; k++) {
            if ((len >> k) & 1) {
                res = op(res, acc[k][v]);
                v = par[k][v];
            }
        }

        return res;
    }

    // u-v パス上の辺値の可換モノイド積。
    S fold_path(int u, int v) const {
        int w = lca(u, v);
        return op(fold_to_ancestor(u, w), fold_to_ancestor(v, w));
    }

    bool is_ancestor(int a, int b) const {
        return tin[a] <= tin[b] and tout[b] <= tout[a];
    }

    int parent(int v) const {
        return par[0][v];
    }

    const vector<int>& children(int v) const {
        return child[v];
    }

    const vector<int>& order() const {
        return ord;
    }

    int depth(int v) const {
        return dep[v];
    }

    int in(int v) const {
        return tin[v];
    }

    int out(int v) const {
        return tout[v];
    }

    int size(int v) const {
        return sub_size[v];
    }

    pair<int, int> subtree_range(int v) const {
        return {tin[v], tout[v]};
    }
};

long long op(long long a, long long b) {
    return a + b;
}

long long e() {
    return 0;
}
