// char_kinds : 文字の種類数
// base : 基準となる文字の文字コード
template<int char_kinds, int base>
struct Trie{
    struct Node{
        vector<int> next; // 子のnodeのindex
        vector<int> str_id; // このnodeで終わる文字列のid
        int c; // nodeに対応する文字の文字コード
        int strs; // 頂点を共有する文字列の数 - base
        Node(int t){
            c = t;
            strs = 0;
            next.assign(char_kinds, -1);
        }
    };

    vector<Node> nodes;
    Trie(){
        nodes.push_back(Node(-1));
    }

    void insert(string &S, int S_id){
        int node_index = 0;
        for(int i = 0; i < S.size(); i ++){
            int c = S[i] - base;
            int &next_index = nodes[node_index].next[c];
            if(next_index == -1){
                next_index = nodes.size();
                nodes.push_back(Node(c));
            }
            nodes[node_index].strs ++;
            node_index = next_index;
        }
        nodes[node_index].strs ++;
        nodes[node_index].str_id.push_back(S_id);
    }

    void insert(string &S){
        insert(S, nodes[0].strs);
    }

    bool search(string &S){
        int node_index = 0;
        for(int i = 0; i < S.size(); i ++){
            int c = S[i] - base;
            int next_index = nodes[node_index].next[c];
            if(next_index == -1){
                return false;
            }
            node_index = next_index;
        }
        return nodes[node_index].str_id.size() > 0;
    }

    int size(){
        return nodes[0].strs;
    }

    int nodes_size(){
        return nodes.size();
    }
};
