// 重み付きグラフ, 開始地点
vector<long long> dijkstra(vector<vector<pair<int, long long>>> &G, int s){
    int N = G.size();
    vector<long long> dist(N, 1e18);
    vector<bool> decided(N, false);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> Q;
    dist[s] = 0;
    Q.push({dist[s], s});
    while(!Q.empty()){
        int pos = Q.top().second;
        Q.pop();
        if(decided[pos]) continue;
        decided[pos] = true;
        rep(i, G[pos].size()){
            int next = G[pos][i].first;
            long long cost = G[pos][i].second;
            if(dist[next] > dist[pos] + cost){
                dist[next] = dist[pos] + cost;
                Q.push({dist[next], next});
            }
        }
    }
    return dist;
}
