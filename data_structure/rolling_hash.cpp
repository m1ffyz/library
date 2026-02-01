const unsigned long long MOD = (1ULL << 61) - 1;

unsigned long long mul_mod(unsigned long long a, unsigned long long b){
    __int128_t t = (__int128_t) a * b;
    unsigned long long t_low = (unsigned long long) t;
    unsigned long long t_high = (unsigned long long) (t >> 61);

    const unsigned long long MASK61 = MOD;
    unsigned long long res = (t & MASK61) + (t >> 61);
    if(res >= MOD){
        res -= MOD;
    }
    return res;
}

unsigned long long generate_base(){
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<unsigned long long> dist(2e9 + 1, MOD - 2);
    return dist(rng);
}


// single
template<typename T>
struct rolling_hash{
private:
    vector<unsigned long long> hash;
    vector<unsigned long long> power;
    unsigned long long base;
    int n;

public:
    rolling_hash() : base(generate_base()), n(0) {}
    rolling_hash(unsigned long long _base) : base(_base), n(0) {}

    void build(const vector<T> &v){
        n = v.size();
        hash.assign(n + 1, 0);
        power.assign(n + 1, 0);
        power[0] = 1;

        for(int i = 0; i < n; i ++){
            power[i + 1] = mul_mod(power[i], base);
            unsigned long long val = (unsigned long long) v[i] % MOD;
            hash[i + 1] = mul_mod(hash[i], base) + val;
            if(hash[i + 1] >= MOD){
                hash[i + 1] -= MOD;
            }
        }
    }

    void build(const string &s){
        vector<char> v(s.begin(), s.end());
        build(v);
    }

    // [l, r) のハッシュ値を取得
    unsigned long long query(int l, int r) const {
        if(l >= r) return 0;
        unsigned long long res = hash[r] + MOD - mul_mod(hash[l], power[r - l]);
        if(res >= MOD){
            res -= MOD;
        }
        return res;
    }

    // 単体のハッシュ値を取得 (val * (base ^ 0))
    unsigned long long get_single_hash (T val) const {
        return (unsigned long long) val % MOD;
    }

    // ハッシュの結合 (h1 * (base ^ len2) + h2)
    unsigned long long combine(unsigned long long h1, unsigned long long h2, int len2) const {
        unsigned long long res = mul_mod(h1, power[len2]) + h2;
        if(res >= MOD){
            res -= MOD;
        }
        return res;
    }

    // [l1, n], [l2, n] における最長共通接頭辞長
    int lcp(int l1, int l2) const {
        int len = min(n - l1, n - l2);
        int low = 0, high = len + 1;
        while(high - low > 1){
            int mid = (low + high) / 2;
            if(query(l1, l1 + mid) == query(l2, l2 + mid)){
                low = mid;
            }else{
                high = mid;
            }
        }
        return low;
    }

    unsigned long long get_base() const {
        return base;
    }

    vector<unsigned long long> get_powers() {
        return power;
    }

    vector<unsigned long long> &get_hash_table(){
        return hash;
    }

    int size() const {
        return n;
    }
};

// double
template<typename T>
struct double_rolling_hash{
public:
    struct hash_pair{
        unsigned long long hash1;
        unsigned long long hash2;

        hash_pair() : hash1(0), hash2(0) {}
        hash_pair(unsigned long long _hash1, unsigned long long _hash2) : hash1(_hash1), hash2(_hash2) {}

        bool operator == (const hash_pair &other){
            return hash1 == other.hash1 and hash2 == other.hash2;
        }

        bool operator != (const hash_pair &other){
            return !(*this == other);
        }

        hash_pair operator + (const hash_pair &other) const {
            unsigned long long res1 = hash1 + other.hash1;
            unsigned long long res2 = hash2 + other.hash2;
            if(res1 >= MOD) res1 -= MOD;
            if(res2 >= MOD) res2 -= MOD;
            return hash_pair(res1, res2);
        }

        hash_pair operator - (const hash_pair &other) const {
            unsigned long long res1 = hash1 + MOD - other.hash1;
            unsigned long long res2 = hash2 + MOD - other.hash2;
            if(res1 >= MOD) res1 -= MOD;
            if(res2 >= MOD) res2 -= MOD;
            return hash_pair(res1, res2);
        }
    };

private:
    rolling_hash<T> rh1;
    rolling_hash<T> rh2;
    vector<T> src;

public:
    double_rolling_hash() : rh1(), rh2(){
        while(rh1.get_base() == rh2.get_base()){
            rh2 = rolling_hash<T> ();
        }
    }

    double_rolling_hash(hash_pair bases) : rh1(bases.hash1), rh2(bases.hash2){}

    hash_pair get_bases() const {
        return hash_pair(rh1.get_base(), rh2.get_base());
    }

    void build(vector<T> &v){
        rh1.build(v);
        rh2.build(v);
        src = v;
    }

    void build(string &s){
        rh1.build(s);
        rh2.build(s);
        src = vector<char> (s.begin(), s.end());
    }

    // [l, r) のハッシュ値を取得
    hash_pair query(int l, int r) const {
        return hash_pair(rh1.query(l, r), rh2.query(l, r));
    }

    // 単体のハッシュ値を取得 (val * (base ^ 0))
    hash_pair get_single_hash(T val) const {
        return hash_pair(rh1.get_single_hash(val), rh2.get_single_hash(val));
    }

    // ハッシュの結合 (h1 * (base ^ len2) + h2)
    hash_pair combine(hash_pair hp1, hash_pair hp2, int len2) const {
        return hash_pair(rh1.combine(hp1.hash1, hp2.hash1, len2), rh2.combine(hp1.hash2, hp2.hash2, len2));
    }

    // [l1, n], [l2, n] における最長共通接頭辞長
    int lcp(int l1, int l2) const {
        int n = rh1.size();
        int len = min(n - l1, n - l2);
        int low = 0, high = len + 1;
        while(high - low > 1){
            int mid = (low + high) / 2;
            if(query(l1, l1 + mid) == query(l2, l2 + mid)){
                low = mid;
            }else{
                high = mid;
            }
        }
        return low;
    }

    // [l1, r1), [l2, r2) における最長共通接頭辞長
    int lcp(int l1, int r1, int l2, int r2){
        int len = min(r1 - l1, r2 - l2);
        int low = 0, high = len + 1;
        while(high - low > 1){
            int mid = (low + high) / 2;
            if(query(l1, l1 + mid) == query(l2, l2 + mid)){
                low = mid;
            }else{
                high = mid;
            }
        }
        return low;
    }

    // this[l1, n1], other[l2, n2] における最長共通接頭辞長
    int lcp(int l1, const double_rolling_hash<T> &other, int l2) const {
        assert(this->get_bases() == other.get_bases());
        int n1 = rh1.size();
        int n2 = other.rh1.size();
        int len = min(n1 - l1, n2 - l2);
        int low = 0, high = len + 1;
        while(high - low > 1){
            int mid = (low + high) / 2;
            if(this->query(l1, l1 + mid) == other.query(l2, l2 + mid)){
                low = mid;
            }else{
                high = mid;
            }
        }
        return low;
    }

    // 部分文字列同士を辞書順比較
    // -1 : [l1, r1) < [l2, r2)
    // 0 : [l1, r1) == [l2, r2)
    // 1 : [l1, r1) > [l2, r2)
    int compare(int l1, int r1, int l2, int r2) const {
        int len1 = r1 - l1;
        int len2 = r2 - l2;
        int l = lcp(l1, r1, l2, r2);

        if(l == len1 and l == len2){
            return 0;
        }
        if(l == len1){
            return -1;
        }
        if(l == len2){
            return 1;
        }

        if(src[l1 + l] < src[l2 + l]){
            return -1;
        }else{
            return 1;
        }
    }

    pair<vector<unsigned long long>, vector<unsigned long long>> get_powers() {
        return {rh1.get_powers(), rh2.get_powers()};
    }

    int size() const {
        return rh1.size();
    }
};
