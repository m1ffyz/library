vector<long long> enum_divisors(long long N){
    vector<long long> div;
    for(long long i = 1; i * i <= N; i++){
        if(N % i == 0){
            div.push_back(i);
            if(N / i != i) div.push_back(N / i);
        }
    }
    sort(all(div));
    return div;
}
