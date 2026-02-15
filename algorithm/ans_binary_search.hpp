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
