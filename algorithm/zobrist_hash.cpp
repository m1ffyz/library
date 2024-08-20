template <long long mod>
struct Zobrist_hash{
    vector<long long> hash;
    vector<vector<int>> vecs;
    int N;
    int vec_num;
    vector<vector<int>> hashed_vecs;
    vector<vector<long long>> hash_sum;

    // _vecs: ハッシュ化したい配列の集合
    // max_num: 取りうる最大の数
    Zobrist_hash(vector<vector<int>> _vecs, int max_num){
        vecs = _vecs;
        N = vecs[0].size();
        vec_num = vecs.size();

        // ハッシュ値の初期化
        hash.resize(max_num + 1);
        for(int i = 0; i <= max_num; i ++){
            hash[i] = rand() % mod;
        }

        // ハッシュ化
        hashed_vecs.resize(vec_num, vector<int>(N));
        for(int i = 0; i < vec_num; i ++){
            for(int j = 0; j < N; j ++){
                hashed_vecs[i][j] = hash[vecs[i][j]];
            }
        }

        // ハッシュ値の累積和
        hash_sum.resize(vec_num, vector<long long>(N + 1, 0));
        for(int i = 0; i < vec_num; i ++){
            for(int j = 1; j <= N; j ++){
                hash_sum[i][j] = hash_sum[i][j - 1] + hashed_vecs[i][j - 1];
                hash_sum[i][j] %= mod;
            }
        }

    }

    // 配列 i の区間 [l, r) のハッシュ値を返す
    long long get_hash(int i, int l, int r){
        long long res = hash_sum[i][r] - hash_sum[i][l] + mod;
        res %= mod;
        return res;
    }
};
