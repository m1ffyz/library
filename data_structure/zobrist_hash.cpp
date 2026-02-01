template <long long mod>
struct Zobrist_hash{
    vector<long long> hash;
    map<int, long long> hash_map;
    vector<vector<int>> vecs;
    int N;
    int vec_num;
    vector<vector<long long>> hashed_vecs;
    vector<vector<long long>> hash_sum;
    vector<vector<long long>> hash_xor;

    // 最大数が小さめのとき
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
        hashed_vecs.resize(vec_num, vector<long long> (N));
        for(int i = 0; i < vec_num; i ++){
            for(int j = 0; j < N; j ++){
                hashed_vecs[i][j] = hash[vecs[i][j]];
            }
        }
    }

    // 最大数が大きめのとき
    // _vecs: ハッシュ化したい配列の集合
    Zobrist_hash(vector<vector<int>> _vecs){
        vecs = _vecs;
        N = vecs[0].size();
        vec_num = vecs.size();

        // ハッシュ値の初期化・ハッシュ化
        hashed_vecs.resize(vec_num, vector<long long> (N));
        for(int i = 0; i < vec_num; i ++){
            for(int j = 0; j < N; j ++){
                long long x = vecs[i][j];
                if(hash_map.count(x) == 0){
                    hash_map[x] = rand() % mod;
                }
                hashed_vecs[i][j] = hash_map[x];
            }
        }
    }

    // 累積XORを計算する (集合)
    void run_xor(){
        hash_xor.resize(vec_num, vector<long long> (N + 1, 0));
        for(int i = 0; i < vec_num; i ++){
            set<int> st;
            for(int j = 0; j < N; j ++){
                if(st.count(vecs[i][j])){
                    hash_xor[i][j + 1] = hash_xor[i][j];
                }else{
                    hash_xor[i][j + 1] = hash_xor[i][j] ^ hashed_vecs[i][j];
                    st.insert(vecs[i][j]);
                }
            }
        }
    }

    // 累積和を計算する (多重集合)
    void run_sum(){
        hash_sum.resize(vec_num, vector<long long> (N + 1, 0));
        for(int i = 0; i < vec_num; i ++){
            for(int j = 0; j < N; j ++){
                hash_sum[i][j + 1] = hash_sum[i][j] + hashed_vecs[i][j];
                hash_sum[i][j + 1] %= mod;
            }
        }
    }
    
    // 配列 i の区間 [l, r) の集合としてのハッシュ値を返す
    long long get_xor_hash(int i, int l, int r){
        long long res = hash_xor[i][r] ^ hash_xor[i][l];
        return res;
    }

    // 配列 i の区間 [l, r) の多重集合としてのハッシュ値を返す
    long long get_sum_hash(int i, int l, int r){
        long long res = hash_sum[i][r] - hash_sum[i][l] + mod;
        res %= mod;
        return res;
    }
};
