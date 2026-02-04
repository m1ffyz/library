/* @file potentialized_dsu.hpp
 * @brief potential 付き Union-Find
 * @author m1ffyz
 * @date 2026-02-04
 * @see https://github.com/m1ffyz/library/blob/main/data_structure/potentialized_dsu.hpp
 */


template <class S, S(*op)(S, S), S (*e)(), S (*inv)(S)>
struct potentialized_dsu {
public:
    potentialized_dsu() : _n(0) {}
    explicit potentialized_dsu(int n) : _n(n), parent_or_size(n, -1), diff_potential(n, e()) {}

    int leader(int a) {
        assert(0 <= a and a < _n);
        if (parent_or_size[a] < 0) {
            return a;
        }
        int root = leader(parent_or_size[a]);

        // 経路圧縮
        // diff_potential[a] = P(a) - P(parent)
        // P(a) - P(root) = (P(parent) - P(root)) + (P(a) - P(parent))
        diff_potential[a] = op(diff_potential[parent_or_size[a]], diff_potential[a]);
        parent_or_size[a] = root;
        return root;
    }

    // 返り値 : 矛盾なし -> leader の idx, 矛盾 -> -1
    int merge(int a, int b, S x) {
        assert(0 <= a and a < _n);
        assert(0 <= b and b < _n);

        int root_a = leader(a);
        int root_b = leader(b);

        // P(b) = P(a) + x
        // P(a) = P(root_a) + diff[a]
        // P(b) = P(root_b) + diff[b]
        // 整理すると,
        // P(root_b) - P(root_a) = diff[a] + x - diff[b]
        x = op(diff_potential[a], x);
        x = op(x, inv(diff_potential[b]));
    
        if (root_a == root_b) {
            if (x == e()) {
                return root_a;
            } else {
                return -1;
            }
        }

        if (-parent_or_size[root_a] < -parent_or_size[root_b]) {
            swap(root_a, root_b);
            x = inv(x);
        }

        parent_or_size[root_a] += parent_or_size[root_b];
        parent_or_size[root_b] = root_a;
        diff_potential[root_b] = x;

        return root_a;
    }

    bool same(int a, int b) {
        assert(0 <= a and a < _n);
        assert(0 <= b and b < _n);
        return leader(a) == leader(b);
    }

    S diff(int a, int b) {
        assert(same(a, b));
        // P(b) - P(a) = (P(b) - P(root)) - (P(a) - P(root))
        //             = diff[b] - diff[a]
        return op(inv(diff_potential[a]), diff_potential[b]);
    }

    int size(int a) {
        assert(0 <= a and a < _n);
        return -parent_or_size[leader(a)];
    }

    vector<vector<int>> groups() {
        vector<int> leader_buf(_n), group_size(_n);
        for (int i = 0; i < _n; i ++) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]] ++;
        }
        vector<vector<int>> result(_n);
        for (int i = 0; i < _n; i ++) {
            result[i].reserve(group_size[i]);
        }
        for (int i = 0; i < _n; i ++) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(remove_if(result.begin(), result.end(), [&](const vector<int> &v) {
            return v.empty();
        }), result.end());
        return result;
    }

private:
    int _n;
    vector<int> parent_or_size;
    vector<S> diff_potential;
};

long long op(long long a, long long b) {
    return a + b;
}

long long e() {
    return 0;
}

long long inv(long long a) {
    return -a;
}
