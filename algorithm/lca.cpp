struct LCA{
    vector<vector<int>> parent;
    vector<int> dist;

    LCA(vector<vector<int>> &G, int root = 0){
        int N = G.size();
        int h = 1;
        while((1 << h) < N) h ++;
        parent.assign(h, vector<int> (N, -1));
        dist.assign(N, -1);
        dfs(G, root, -1, 0);
        for(int i = 0; i + 1 < h; i ++){
            for(int v = 0; v < N; v ++){
                if(parent[i][v] < 0) parent[i + 1][v] = -1;
                else parent[i + 1][v] = parent[i][parent[i][v]];
            }
        }
    }

    void dfs(vector<vector<int>> &G, int v, int p, int d){
        parent[0][v] = p;
        dist[v] = d;
        for(int u : G[v]){
            if(u == p) continue;
            dfs(G, u, v, d + 1);
        }
    }

    int same_parent(int u, int v){
        if(dist[u] < dist[v]) swap(u, v);
        int N = parent.size();
        for(int i = 0; i < N; i ++){
            if((dist[u] - dist[v]) >> i & 1) u = parent[i][u];
        }
        if(u == v) return u;
        for(int i = N - 1; i >= 0; i --){
            if(parent[i][u] != parent[i][v]){
                u = parent[i][u];
                v = parent[i][v];
            }
        }
        return parent[0][u];
    }

    int get_dist(int u, int v){
        return dist[u] + dist[v] - 2 * dist[same_parent(u, v)];
    }
};
