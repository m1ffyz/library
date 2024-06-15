bool is_prime(long long N){
    if(N == 1) return false;
    for(long long i = 2; i * i <= N; i++){
        if(N % i == 0) return false;
    }
    return true;
}
