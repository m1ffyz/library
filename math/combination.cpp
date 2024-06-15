vector<vector<long long>> comb_memo(50, vector<long long> (50, -1));
long long comb(int n, int r){
    if(n < 1 and r < 0 and n < r) return (long long) -1e14;
    if(comb_memo[n][r] > 0) return comb_memo[n][r];
    if(r == 0 or r == n){
        comb_memo[n][0] = 1;
        comb_memo[n][r] = 1;
        return 1;
    }
    long long ret = comb(n - 1, r - 1) + comb(n - 1, r);
    comb_memo[n][r] = ret;
    comb_memo[n][n - r] = ret;
    return ret;
}
