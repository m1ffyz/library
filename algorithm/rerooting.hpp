/**
 * @file rerooting.hpp
 * @brief 全方位木DP
 * @author m1ffyz
 * @date 2026-02-02
 * @see https://github.com/m1ffyz/library/blob/main/algorithm/rerooting.hpp
 */


// S : データの型
// op : S op(S a, S, b) 
//     -> この結果同士のマージ方法 (例: max, +)
// e : 単位元
// put : S put(S val, int v, int p)
//     -> 頂点 v から頂点 p に移動する場合に, 値 val をどう変更するか (例: val + 1)

// op と put
// op -> 複数の子からの値をひとつにまとめる
// put -> 子から親へ辺を進むときの値の変化

template <class S, S (*op)(S, S), S (*e)(), S (*put)(S, int, int)>
struct rerooting {

    int N;
    vector<vector<int>> adj;
    
    vector<S> dp, ans;

    rerooting(int n) : N(n) {
        adj.resize(N);
        dp.resize(n, e());
        ans.resize(n, e());
    }
    
    rerooting(const vector<vector<int>> &g) : N(g.size()), adj(g) {
        dp.resize(N, e());
        ans.resize(N, e());
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void build() {
        vector<int> parent(N, -1);
        vector<int> order;
        order.reserve(N);

        vector<int> stack = {0};
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            order.push_back(v);

            for (int i = (int) adj[v].size() - 1; i >= 0; i --) {
                int u = adj[v][i];
                if (u == parent[v]) continue;
                parent[u] = v;
                stack.push_back(u);
            }
        }

        for (int i = N - 1; i >= 0; i --) {
            int v = order[i];
            S res = e();
            for (int u : adj[v]) {
                if (u == parent[v]) continue;
                res = op(res, put(dp[u], u, v));
            }
            dp[v] = res;
        }

        vector<S> values_from_parent(N, e());
        for (int v : order) {
            vector<S> values;
            if (parent[v] != -1) {
                values.push_back(values_from_parent[v]);
            }
            for (int u : adj[v]) {
                if (u == parent[v]) continue;
                values.push_back(put(dp[u], u, v));
            }

            int deg = (int) values.size();
            vector<S> left(deg + 1, e()), right(deg + 1, e());
            for (int i = 0; i < deg; i ++) {
                left[i + 1] = op(left[i], values[i]);
            }
            for (int i = deg - 1; i >= 0; i --) {
                right[i] = op(values[i], right[i + 1]);
            }

            ans[v] = put(left[deg], v, -1);

            int idx = 0;
            int child_idx_offset = (parent[v] != -1) ? 1 : 0;
            for (int u : adj[v]) {
                if (u == parent[v]) continue;

                int current_vec_idx = idx + child_idx_offset;
                S val_for_child = op(left[current_vec_idx], right[current_vec_idx + 1]);
                values_from_parent[u] = put(val_for_child, v, u);
                idx ++;
            }
        }
    }

    S query(int v){
        return ans[v];
    }

};
