bool check_palindrome(string S){
    int N = S.size();
    rep(i, N / 2) if(S[i] != S[N - 1 - i]) return false;
    return true;
}
