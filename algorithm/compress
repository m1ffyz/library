template<typename T>
struct Compress{
    vector<T> coords;
    vector<int> idxs;

    Compress(vector<T> vec){
        coords = vec;
        idxs.resize(vec.size());
        sort(coords.begin(), coords.end());
        coords.erase(unique(coords.begin(), coords.end()), coords.end());
        for (int i = 0; i < idxs.size(); i++) {
            idxs[i] = lower_bound(coords.begin(), coords.end(), vec[i]) - coords.begin();
        }
    }

    // 圧縮前value -> 圧縮後index
    int get_idx(T val){
        auto itr = lower_bound(coords.begin(), coords.end(), val);
        int num = itr - coords.begin();
        return num;
    }

    // 圧縮後index -> 圧縮前value
    T get_coord(int idx){
        return coords[idx];
    }

    // 圧縮前index -> 圧縮後index
    int new_idx(int idx){
        return idxs[idx];
    }

    // 圧縮後の配列
    vector<int> new_vector(){
        return idxs;
    }
};
