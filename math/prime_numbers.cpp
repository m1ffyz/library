// N 以下の素数を羅列する
vector<int> prime_numbers;
void make_prime(int N){
    vector<bool> isprime(N + 1, true);
    isprime[1] = false;
    for(int i = 2; i <= N; i++){
        if(isprime[i]){
            for(int j = 2 * i; j <= N; j += i){
                isprime[j] = false;
            }
            prime_numbers.push_back(i);
        }
    }
}
