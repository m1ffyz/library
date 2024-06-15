// 10進数 -> n進数
vector<int> base10_to_basen(long long x, long long n){
    vector<int> ans;
    while(x > 0){
        ans.push_back(x % n);
        x /= n;
    }
    return ans;
}

// n進数 -> 10進数
long long basen_to_base10(vector<int> a, long long n){
    int N = a.size();
    int c = 1;
    long long ans = 0;
    rep(i, N){
        ans += c * a[i];
        c *= n;
    }
    return ans;
}

// n進数 -> m進数
vector<int> basen_to_basem(vector<int> a, long long n, long long m){
    return base10_to_basen(basen_to_base10(a, n), m);
}
