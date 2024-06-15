vector<pair<long long, long long>> prime_factorize(long long N){
    vector<pair<long long, long long>> prime;
    for(long long i = 2; i * i <= N; i++){
        if(N % i != 0) continue;
        long long e = 0;
        while(N % i == 0){
            e++;
            N /= i;
        }
        prime.push_back({i, e});
    }
    if(N != 1) prime.push_back({N, 1});
    return prime;
}
