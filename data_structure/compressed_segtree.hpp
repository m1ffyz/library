/* @file compressed_segtree.hpp
 * @brief 座標圧縮 segment tree
 * @author m1ffyz
 * @date 2026-02-16
 * @see https://github.com/m1ffyz/library/blob/main/data_structure/compressed_segtree.hpp
 */

#include <atcoder/segtree>

template <class S, S (*op)(S, S), S (*e)(), class T = long long>
struct compressed_segtree {
    vector<T> coords;
    atcoder::segtree<S, op, e> seg;

    void add_coord(T x) {
        coords.push_back(x);
    }

    void build() {
        sort(coords.begin(), coords.end());
        coords.erase(unique(coords.begin(), coords.end()), coords.end());
        seg = atcoder::segtree<S, op, e>(coords.size());
        built = true;
    }

    void set(T x, S val) {
        int id = get_id(x);
        assert(id < (int) coords.size() and coords[id] == x);
        seg.set(id, val);
    }

    S get(T x) {
        int id = get_id(x);
        if (id < (int) coords.size() and coords[id] == x) {
            return seg.get(id);
        }
        return e();
    }

    S prod(T l, T r) {
        assert(l <= r);
        return seg.prod(get_id(l), get_id(r));
    }

    S all_prod() {
        assert(built);
        return seg.all_prod();
    }

    int size() {
        assert(built);
        return coords.size();
    }

private:
    bool built = false;

    int get_id(T x) {
        assert(built);
        return lower_bound(coords.begin(), coords.end(), x) - coords.begin();
    }
};
