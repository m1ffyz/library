/* @file ans_binary_search.hpp
 * @brief 答えで二分探索
 * @author m1ffyz
 * @date 2026-02-15
 * @see https://github.com/m1ffyz/library/blob/main/algorithm/ans_binaary_search.hpp
 */

bool ans_check(long long x) {


    
}

long long ans_binary_search() {
    long long left = -1, right = 1e18;

    long long ok, ng;
    if (ans_check(right)) {
        ok = right;
        ng = left;
    } else {
        ok = left;
        ng = right;
    }

    while (abs(ok - ng) > 1) {
        long long mid = ok + (ng - ok) / 2;

        // デバッグ用
        // cout << "debug: ok = " << ok << ", mid = " << mid << ", ng = " << ng << "\n";

        if (ans_check(mid)) {
            ok = mid;
        } else {
            ng = mid;
        }
    }

    return ok;
}
