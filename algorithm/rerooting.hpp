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
        dfs_bottom_up(0, -1);
        dfs_top_down(0, -1, e());
    }

    S query(int v){
        return ans[v];
    }

private:
    void dfs_bottom_up(int v, int p) {
        S res = e();
        for (int u : adj[v]){
            if (u == p) continue;
            dfs_bottom_up(u, v);
            // u から v へ値を渡す
            res = op(res, put(dp[u], u, v));
        }
        dp[v] = res;
    }

    void dfs_top_down(int v, int p, S val_p) {
        vector<S> children_values;
        if (p != -1) {
            children_values.push_back(val_p);
        }
        for (int u : adj[v]) {
            if (u == p) continue;
            // u から v へ渡される値を集める
            children_values.push_back(put(dp[u], u, v));
        }

        S total_res = e();
        for (const auto &val : children_values) {
            total_res = op(total_res, val);
        }
        // 最終結果: v から -1 (どこにも行かない)
        ans[v] = put(total_res, v, -1);

        int deg = children_values.size();
        vector<S> left(deg + 1, e()), right(deg + 1, e());
        
        for (int i = 0; i < deg; i ++) {
            left[i + 1] = op(left[i], children_values[i]);
        }
        for (int i = deg - 1; i >= 0; i --) {
            right[i] = op(children_values[i], right[i + 1]);
        }

        int idx = 0;
        int child_idx_offset = (p != -1) ? 1 : 0;

        for (int u: adj[v]) {
            if(u == p) continue;

            int current_vec_idx = idx + child_idx_offset;
            S val_for_child = op(left[current_vec_idx], right[current_vec_idx + 1]);
            
            // v から u へ値を渡す
            dfs_top_down(u, v, put(val_for_child, v, u));
            idx ++;
        }
    }
};

struct S {
    int val;
};


S op(S a, S b) {
    
}

S e() {
    
}

S put(S val, int v, int p) {
    
}
