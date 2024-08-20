struct Doubling{
    int N;
    vector<int> next;
    vector<vector<int>> dp;

    // dp[i][j] := j から 2^i 回移動した先の頂点
    // O(N log(k)) (k = 64)
    Doubling(vector<int> next_){
        N = next_.size();
        next = next_;
        dp.assign(64, vector<int>(N, -1));
        dp[0] = next;
        for(int i = 1; i < 64; i ++){
            for(int j = 0; j < N; j ++){
                dp[i][j] = dp[i - 1][dp[i - 1][j]];
            }
        }
    }

    // p から k 回移動した先の頂点
    int pos(int p, long long k){
        for(int i = 0; i < 64; i ++){
            if(k & (1LL << i)){
                p = dp[i][p];
            }
        }
        return p;
    }

    // すべての頂点について k 回移動した先の頂点
    vector<int> all_pos(long long k){
        vector<int> res(N);
        for(int i = 0; i < N; i ++){
            res[i] = pos(i, k);
        }
        return res;
    }
};
