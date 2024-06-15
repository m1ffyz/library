pair<string, vector<int>> rle(string &S){
    int N = S.size();
    if(S.empty()) return {};
    pair<string, vector<int>> ret;
    ll cnt = 0;
    char c = S[0];
    rep(i, N){
        if(S[i] == c){
            cnt ++;
        }else{
            ret.first += c;
            ret.second.push_back(cnt);
            c = S[i];
            cnt = 1;
        }
    }
    if(cnt > 0){
        ret.first += c;
        ret.second.push_back(cnt);
    }
    return ret;
}
