/**
 * @file rle.hpp
 * @brief ランレングス圧縮
 * @author m1ffyz
 * @date 2026-02-14
 * @see https://github.com/m1ffyz/library/blob/main/data_structure/rle.hpp
 */

template <typename Container>
vector<pair<typename Container::value_type, long long>> rle(const Container &A) {
    using T = typename Container::value_type;
    vector<pair<T, long long>> res;

    if (A.empty()) {
        return res;
    }

    long long cnt = 0;
    T current_val = A[0];

    for (const auto &x : A) {
        if (x == current_val) {
            cnt ++;
        } else {
            res.push_back({current_val, cnt});
            current_val = x;
            cnt = 1;
        }
    }

    if (cnt > 0) {
        res.push_back({current_val, cnt});
    }

    return res;
} 
