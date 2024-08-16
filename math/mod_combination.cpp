// 二項係数 nCk mod p を求める
// ACL 使用
// 前処理 comb_process を実行する必要がある
// 前処理 O(comb_max), クエリ処理 O(1)

vector<mint> fac, fac_inv, inv;

// 前処理 O(comb_max)
// comb_max : n の最大値
void comb_process(int comb_max){
    comb_max += 1;
    fac.resize(comb_max);
    fac_inv.resize(comb_max);
    inv.resize(comb_max);
    int M = mint::mod();
    fac[0] = 1; fac[1] = 1;
    fac_inv[0] = 1; fac_inv[1] = 1;
    inv[1] = 1;
    for(int i = 2; i < comb_max; i ++){
        fac[i] = fac[i - 1] * i;
        inv[i] = M - inv[M % i] * (M / i);
        fac_inv[i] = fac_inv[i - 1] * inv[i];
    }
}

// クエリ処理 O(1)
// nCk を求める
mint comb(int n, int k){
    if(n < k) return 0;
    if(n < 0 or k < 0) return 0;
    return fac[n] * fac_inv[k] * fac_inv[n - k];
}
