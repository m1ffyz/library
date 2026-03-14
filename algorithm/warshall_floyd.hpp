struct WarshallFloyd{
    vector<vector<pair<int, long long>>> G;
    int N;
    vector<vector<long long>> dist;

    WarshallFloyd(int n){
        N = n;
        G.resize(N);
        dist.assign(N, vector<long long>(N, 1e18));
    }

    WarshallFloyd(vector<vector<pair<int, long long>>> g){
        N = g.size();
        G = g;
        dist.assign(N, vector<long long>(N, 1e18));
    }

    void add(int x, int y, long long cost){
        G[x].push_back({y, cost});
        G[y].push_back({x, cost});
    }

    void add_dir(int x, int y, long long cost){
        G[x].push_back({y, cost});
    }

    void run(){
        for(int i = 0; i < N; i ++){
            dist[i][i] = 0;
            for(auto [to, cost] : G[i]){
                dist[i][to] = min(dist[i][to], cost);
            }
        }
        for(int k = 0; k < N; k ++){
            for(int i = 0; i < N; i ++){
                for(int j = 0; j < N; j ++){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    void add_later(int x, int y, long long cost){
        G[x].push_back({y, cost});
        G[y].push_back({x, cost});
        dist[x][y] = min(dist[x][y], cost);
        dist[y][x] = min(dist[y][x], cost);

        for(int i = 0; i < N; i ++){
            for(int j = 0; j < N; j ++){
                dist[i][j] = min(dist[i][j], dist[i][x] + dist[x][y] + dist[y][j]);
                dist[i][j] = min(dist[i][j], dist[i][y] + dist[y][x] + dist[x][j]);
            }
        }
    }

    vector<vector<long long>> get_all_dist(){
        return dist;
    }

    long long get_dist(int x, int y){
        return dist[x][y];
    }
};
