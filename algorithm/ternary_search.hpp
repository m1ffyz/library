/* @file ternary_search.hpp
 * @brief 三分探索
 * @author m1ffyz
 * @date 2026-02-21
 * @see https://github.com/m1ffyz/library/blob/main/algorithm/ternary_search.hpp
 * @note Created by Gemini 3.1 Pro
 */

// T: 定義域の型 (int, long long, double など)
// F: 評価関数の型 (ラムダ式を想定)
// 戻り値: {最適解となる x, そのときの f(x) の値}
template <typename T, typename F>
auto ternary_search(T left, T right, bool find_max, F f) {
    using ValType = decltype(f(left));

    // 1. 整数型 (int, long long など) の場合
    if constexpr (std::is_integral_v<T>) {
        while (right - left > 2) {
            T m1 = left + (right - left) / 3;
            T m2 = right - (right - left) / 3;

            ValType v1 = f(m1);
            ValType v2 = f(m2);

            bool go_right = find_max ? (v1 < v2) : (v1 > v2);

            if (go_right) {
                left = m1;
            } else {
                right = m2;
            }
        }

        T best_x = left;
        ValType best_val = f(left);
        for (T x = left + 1; x <= right; ++x) {
            ValType val = f(x);
            if (find_max ? (best_val < val) : (best_val > val)) {
                best_val = val;
                best_x = x;
            }
        }
        return std::pair<T, ValType>{best_x, best_val};
    } 
    // 2. 浮動小数点型 (double, long double など) の場合
    else if constexpr (std::is_floating_point_v<T>) {
        int iterations = 100;
        for (int i = 0; i < iterations; ++i) {
            T m1 = left + (right - left) / 3.0;
            T m2 = right - (right - left) / 3.0;

            ValType v1 = f(m1);
            ValType v2 = f(m2);

            bool go_right = find_max ? (v1 < v2) : (v1 > v2);

            if (go_right) {
                left = m1;
            } else {
                right = m2;
            }
        }
        return std::pair<T, ValType>{left, f(left)};
    }
}
