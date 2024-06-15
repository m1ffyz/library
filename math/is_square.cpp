bool is_square(long long N){
    for(long long i = 1; i * i <= N; i++){
        if(i * i == N) return true;
    }
    return false;
}
