struct LCS{
    string S, T;
    vector<vector<int>> dp;
    int N, M;

    LCS(string S, string T) : S(S), T(T){
        N = S.size(), M = T.size();
        dp.assign(N + 1, vector<int> (M + 1, 0));
        for(int i = 0; i <= N; i ++) dp[i][0] = 0;
        for(int j = 0; j <= M; j ++) dp[0][j] = 0;
        for(int i = 1; i <= N; i ++){
            for(int j = 1; j <= M; j ++){
                dp[i][j] = max(dp[i - 1][j - 1] + (S[i - 1] == T[j - 1]), max(dp[i - 1][j], dp[i][j - 1]));
            }
        }
    }

    int lcs_size(){
        return dp[N][M];
    }

    string lcs(){
        string res = "";
        int i = N, j = M;
        while(i > 0 && j > 0){
            if(S[i - 1] == T[j - 1]){
                res += S[i - 1];
                i--;
                j--;
            }
            else if(dp[i - 1][j] > dp[i][j - 1]){
                i--;
            }
            else{
                j--;
            }
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
