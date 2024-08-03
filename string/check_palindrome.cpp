bool check_palindrome(string &S){
    for(int i = 0; i < S.size() / 2; i ++){
        if(S[i] != S[S.size() - 1 - i]){
            return false;
        }
    }
    return true;
}

// p: 開始位置,  l: 長さ
bool check_palindrome(string &S, int p, int l){
    if(p + l - 1 >= S.size()) return false;
    for(int i = 0; i < l / 2; i ++){
        if(S[p + i] != S[p + l - 1 - i]){
            return false;
        }
    }
    return true;
}
