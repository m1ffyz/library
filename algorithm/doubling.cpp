/* @file doubling.hpp
 * @brief ダブリング
 * @author m1ffyz
 * @date 2026-02-15
 * @see https://github.com/m1ffyz/library/blob/main/algorithm/doubling.hpp
 */

template <class S, S (*op)(S, S), S (*e)()>
struct doubling {
    int N;
    int LOG;
    vector<vector<int>> next;
    vector<vector<S>> val;

    struct res{
        int pos;
        S val;
    };
    
    doubling(int N, long long max_k) : N(N) {
        LOG = 1;
        while ((1LL << LOG) <= max_k) {
            LOG ++;
        }
        next.assign(LOG, vector<int> (N, -1));
        val.assign(LOG, vector<S> (N, e()));
    }

    void set_next(int v, int t, S x) {
        next[0][v] = t;
        val[0][v] = x;
    }

    void build() {
        for (int i = 0; i < LOG - 1; i ++) {
            for (int v = 0; v < N; v ++) {
                if (next[i][v] == -1) {
                    val[i + 1][v] = val[i][v];
                    continue;
                }

                next[i + 1][v] = next[i][next[i][v]];
                val[i + 1][v] = op(val[i][v], val[i][next[i][v]]);
            }
        }
    }

    res query(int p, long long k) {
        S ans = e();
        int curr = p;
        for (int i = 0; i < LOG; i ++) {
            if ((k >> i) & 1) {
                if (curr == -1) {
                    break;
                }

                ans = op(ans, val[i][curr]);
                curr = next[i][curr];
            }
        }

        return {curr, ans};
    }
};
