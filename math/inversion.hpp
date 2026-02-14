/**
 * @file inversion.hpp
 * @brief 転倒数
 * @author m1ffyz
 * @date 2026-02-14
 * @see https://github.com/m1ffyz/library/blob/main/math/inversion.hpp
 */

#include <atcoder/fenwicktree>

template <typename T>
struct inversion {
    long long total;
    vector<long long> total_per_element;
    vector<long long> left_inversions;
    vector<long long> right_inversions;
};

template <typename T>
inversion<T> calc_inversion (const vector<T> &A, bool asc = true) {
    int n = A.size();
    inversion<T> res;
    res.total = 0;
    res.total_per_element.assign(n, 0);
    res.left_inversions.assign(n, 0);
    res.right_inversions.assign(n, 0);

    if (n <= 1) {
        return res;
    }

    vector<T> sorted_A = A;
    sort(sorted_A.begin(), sorted_A.end());
    sorted_A.erase(unique(sorted_A.begin(), sorted_A.end()), sorted_A.end());
    int m = sorted_A.size();

    atcoder::fenwick_tree<long long> fw_left(m);

    for (int i = 0; i < n; i ++) {
        int rank = lower_bound(sorted_A.begin(), sorted_A.end(), A[i]) - sorted_A.begin();

        if (asc) {
            res.left_inversions[i] = i - fw_left.sum(0, rank + 1);
        } else {
            res.left_inversions[i] = fw_left.sum(0, rank);
        }

        res.total += res.left_inversions[i];
        fw_left.add(rank, 1);
    }

    atcoder::fenwick_tree<long long> fw_right(m);

    for (int i = n - 1; i >= 0; i --) {
        int rank = lower_bound(sorted_A.begin(), sorted_A.end(), A[i]) - sorted_A.begin();

        if (asc) {
            res.right_inversions[i] = fw_right.sum(0, rank);
        } else {
            int elements_on_right = (n - 1) - i;
            res.right_inversions[i] = elements_on_right - fw_right.sum(0, rank + 1);
        }
    
        fw_right.add(rank, 1);
    }

    for (int i = 0; i < n; i ++) {
        res.total_per_element[i] = res.left_inversions[i] + res.right_inversions[i];
    }

    return res;
}
