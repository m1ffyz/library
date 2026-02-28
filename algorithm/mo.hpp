/**
 * @file mo.hpp
 * @brief Mo アルゴリズム
 * @author m1ffyz
 * @date 2026-02-28
 * @see https://github.com/m1ffyz/library/blob/main/algorithm/mo.hpp
 */

struct Mo {
    int n;
    vector<pair<int, int>> queries;

    Mo(int n) : n(n) {}

    void add_query(int l, int r) {
        queries.emplace_back(l, r);
    }

    // add(idx): idx番目の要素を区間に加える
    // del(idx): idx番目の要素を区間から消す
    // rem(query_idx): query_idx番目のクエリの答えを記録する
    template <typename Add, typename Del, typename Rem>
    void build(const Add &add, const Del &del, const Rem &rem) {
        int q = queries.size();
        if (q == 0) {
            return;
        }

        int bsz = max<int> (1, n / sqrt(q));
        vector<int> order(q);
        iota(order.begin(), order.end(), 0);

        sort(order.begin(), order.end(), [&](int a, int b) {
            int l_blk_a = queries[a].first / bsz;
            int l_blk_b = queries[b].first / bsz;
            if (l_blk_a != l_blk_b) {
                return l_blk_a < l_blk_b;
            }
            if (l_blk_a & 1) {
                return queries[a].second < queries[b].second;
            } else {
                return queries[a].second > queries[b].second;
            }
        });

        int nl = 0, nr = 0;
        for (int idx : order) {
            int l = queries[idx].first;
            int r = queries[idx].second;

            while (nl > l) {
                nl --;
                add(nl);
            }

            while (nr < r) {
                add(nr);
                nr ++;
            }

            while (nl < l) {
                del(nl);
                nl ++;
            }

            while (nr > r) {
                nr --;
                del(nr);
            }

            rem(idx);
        }
    }  
};
