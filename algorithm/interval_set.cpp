struct interval_set{
    const long long INF = 2e18;

    enum type{
        closed,     // [l, r]
        open,       // (l, r)
        half_left,  // [l, r) : default
        half_right  // (l, r]
    };

    // 半開区間 [l, r) の集合
    set<pair<long long, long long>> st;

    // 含まれる整数の個数
    long long total_length = 0;

    // 入力を [l, r) に正規化
    pair<long long, long long> normalize(long long l, long long r, type t){
        if(t == closed){
            return {l, r + 1};
        }
        if(t == open){
            return {l + 1, r};
        }
        if(t == half_left){
            return {l, r};
        }
        if(t == half_right){
            return {l + 1, r + 1};
        }
        return {l, r};
    }

    // コンストラクタ
    interval_set() {}

    interval_set(vector<long long> v){
        if(v.empty()) return;
        
        // ソートして重複削除
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());

        // 連続する値を結合して追加
        long long l = v[0];
        long long r = v[0];
        for(int i = 1; i < v.size(); i ++){
            if(v[i] == r + 1){
                r = v[i];
            }else{
                st.insert({l, r + 1});
                total_length += r + 1 - l;
                l = v[i];
                r = v[i];
            }
        }

        st.insert({l, r + 1});
        total_length += r + 1 - l;
    }

    interval_set(vector<int> v){
        if(v.empty()) return;
        
        // ソートして重複削除
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());

        // 連続する値を結合して追加
        long long l = v[0];
        long long r = v[0];
        for(int i = 1; i < v.size(); i ++){
            if(v[i] == r + 1){
                r = v[i];
            }else{
                st.insert({l, r + 1});
                total_length += r + 1 - l;
                l = v[i];
                r = v[i];
            }
        }

        st.insert({l, r + 1});
        total_length += r + 1 - l;
    }

    // 区間取得: 指定された x を含む区間 [l, r) を返す
    // 含まれない場合は {x, x} を返す
    pair<long long, long long> get_interval(long long x){
        auto it = st.upper_bound({x, INF});
        if(it != st.begin()){
            it --;
            if(it->second > x){
                return *it;
            }
        }
        return {x, x};
    }

    // 点検索: x が含まれているか
    bool contains(long long x){
        auto p = get_interval(x);
        return p.first != p.second;
    }

    // x 以上で集合に含まれない最小の値を返す
    long long mex(long long x = 0){
        auto p = get_interval(x);
        if(p.first != p.second){
            return p.second;
        }
        return x;
    }

    // 区間追加
    void add(long long l, long long r, type t = half_left){
        auto p = normalize(l, r, t);
        l = p.first; r = p.second;
        if(l >= r) return;

        auto it = st.upper_bound({l, -INF}); 
        if(it != st.begin()) it --;

        while(it != st.end()){
            if(it->first > r) break;
            if(it->second < l){
                it ++;
                continue;
            }

            l = min(l, it->first);
            r = max(r, it->second);
            total_length -= (it->second - it->first);
            it = st.erase(it);
        }
        st.insert({l, r});
        total_length += r - l;
    }

    // 区間削除
    void remove(long long l, long long r, type t = half_left){
        auto p = normalize(l, r, t);
        l = p.first; r = p.second;
        if(l >= r) return;

        auto it = st.upper_bound({l, -INF});
        if(it != st.begin()) it--;

        while (it != st.end()) {
            if(it->first >= r) break; 
            if(it->second <= l){     
                it++;
                continue;
            }

            long long ol = it->first;
            long long or_ = it->second;
            total_length -= or_ - ol;
            it = st.erase(it);

            if(ol < l){
                st.insert({ol, l});
                total_length += l - ol;
            }
            if(or_ > r){
                st.insert({r, or_});
                total_length += or_ - r;
            }
        }
    }
    
    // 区間数 (連結成分の数)
    int count(){
        return st.size();
    }
    
    // 集合に含まれる整数の個数 (区間長の総和)
    long long size(){
        return total_length;
    }
};
