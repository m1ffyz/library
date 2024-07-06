struct Dijkstra{
    vector<vector<pair<int, long long>>> G;
    int N;
    int s;
    vector<long long> d;
    vector<int> prev;

    Dijkstra(int n){
        N = n;
        G.resize(N);
        d.assign(N, 1e18);
        prev.assign(N, -1);
        s = 0;
    }
    
    Dijkstra(vector<vector<pair<int, long long>>> g){
        G = g;
        N = G.size();
        d.assign(N, 1e18);
        prev.assign(N, -1);
        s = 0;
    }

    // 無向グラフを追加
    void add(int x, int y, long long cost){
        G[x].push_back({y, cost});
        G[y].push_back({x, cost});
    }

    // 有向グラフを追加
    void add_dir(int x, int y, long long cost){
        G[x].push_back({y, cost});
    }

    // 点 x から dijkstra を走らせる
    void run(int x){
        s = x;
        vector<bool> decided(N, false);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> Q;
        d[x] = 0;
        Q.push({d[x], x});
        while(!Q.empty()){
            int pos = Q.top().second;
            Q.pop();
            if(decided[pos]) continue;
            decided[pos] = true;
            for(int i = 0; i < G[pos].size(); i ++){
                int nxt = G[pos][i].first;
                long long cst = G[pos][i].second;
                if(d[nxt] > d[pos] + cst){
                    d[nxt] = d[pos] + cst;
                    prev[nxt] = pos;
                    Q.push({d[nxt], nxt});
                }
            }
        }
    }

    // 点 x の距離
    long long dist(int x){
        return d[x];
    }

    // すべての距離
    vector<long long> dist_all(){
        return d;
    }
    
    // 点 x までの経路
    vector<int> path(int x){
        vector<int> ret;
        int p = x;
        while(p >= 0){
            ret.push_back(p);
            p = prev[p];
        }
        if(ret.back() != s){
            ret.assign(1, -1);
        }else{
            reverse(ret.begin(), ret.end());
        }
        return ret;
    }
};
