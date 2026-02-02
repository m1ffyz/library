struct cumulative_sum{
    vector<long long> sums;

    cumulative_sum() {}

    template <typename T>
    cumulative_sum(const vector<T> &data){
        build(data, [](T x){
            return x;
        });
    }

    template <typename Sequence, typename Function>
    cumulative_sum(const Sequence &data, Function func){
        build(data, func);
    }

    template <typename Sequence, typename Function>
    void build(const Sequence &data, Function func){
        int n = data.size();
        sums.assign(n + 1, 0);
        for(int i = 0; i < n; i ++){
            sums[i + 1] = sums[i] + func(data[i]);
        }
    }

    // [l, r) の和
    long long query(int l, int r){
        if(l > r){
            return 0;
        }
        return sums[r] - sums[l];
    }

    vector<long long> get_sums(){
        return sums;
    }
};

struct cumulative_sum_2d{
    vector<vector<long long>> sums;

    cumulative_sum_2d() {}

    template <typename T>
    cumulative_sum_2d(const vector<vector<T>> &data){
        build(data, [](T x){ return x; });
    }

    template <typename Sequence, typename Function>
    void build(const Sequence &data, Function func){
        if (data.empty()) return;
        int H = data.size();
        int W = data[0].size();
        
        sums.assign(H + 1, vector<long long> (W + 1, 0));

        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                sums[i + 1][j + 1] = func(data[i][j]);
            }
        }

        for(int i = 1; i <= H; i++){
            for(int j = 1; j <= W; j++){
                sums[i][j] += sums[i - 1][j];
            }
        }

        for(int i = 1; i <= H; i++){
            for(int j = 1; j <= W; j++){
                sums[i][j] += sums[i][j - 1];
            }
        }
    }

    // [x1, x2) x [y1, y2)
    long long query(int x1, int x2, int y1, int y2) {
        if(x1 >= x2 or y1 >= y2) return 0; 
        return sums[x2][y2] - sums[x1][y2] - sums[x2][y1] + sums[x1][y1];
    }
};

struct cumulative_sum_3d {
    vector<vector<vector<long long>>> sums;

    cumulative_sum_3d() {}

    template <typename T>
    cumulative_sum_3d(const vector<vector<vector<T>>> &data) {
        build(data, [](T x) { return x; });
    }

    template <typename Sequence, typename Function>
    void build(const Sequence &data, Function func) {
        if (data.empty() or data[0].empty()) return;
        int X = data.size();       
        int Y = data[0].size();    
        int Z = data[0][0].size(); 

        sums.assign(X + 1, vector<vector<long long>>(Y + 1, vector<long long>(Z + 1, 0)));

        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++) {
                for (int k = 0; k < Z; k++) {
                    sums[i + 1][j + 1][k + 1] = func(data[i][j][k]);
                }
            }
        }

        for (int i = 1; i <= X; i++) {
            for (int j = 1; j <= Y; j++) {
                for (int k = 1; k <= Z; k++) {
                    sums[i][j][k] += sums[i - 1][j][k];
                }
            }
        }

        for (int i = 1; i <= X; i++) {
            for (int j = 1; j <= Y; j++) {
                for (int k = 1; k <= Z; k++) {
                    sums[i][j][k] += sums[i][j - 1][k];
                }
            }
        }

        for (int i = 1; i <= X; i++) {
            for (int j = 1; j <= Y; j++) {
                for (int k = 1; k <= Z; k++) {
                    sums[i][j][k] += sums[i][j][k - 1];
                }
            }
        }
    }

    // [x1, x2) x [y1, y2) x [z1, z2)
    long long query(int x1, int x2, int y1, int y2, int z1, int z2) {
        if (x1 >= x2 or y1 >= y2 or z1 >= z2) return 0;

        return sums[x2][y2][z2]
             - sums[x1][y2][z2] - sums[x2][y1][z2] - sums[x2][y2][z1]
             + sums[x1][y1][z2] + sums[x1][y2][z1] + sums[x2][y1][z1]
             - sums[x1][y1][z1];
    }
};
