struct LIS{
    int N;
    vector<int> A;
    vector<int> L;

    LIS(vector<int> &a) : N(a.size()), A(a){
        L.assign(N, 1e9);
        for(int i = 0; i < N; i ++){
            *lower_bound(L.begin(), L.end(), A[i]) = A[i];
        }
    }

    int lis_size(){
        return lower_bound(L.begin(), L.end(), 1e9) - L.begin();
    }
};
