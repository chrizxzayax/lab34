// // comsc 210 | lab33 | Christian Molina
#include <iostream>
#include <vector>
#include <queue>
#include <stack>


using namespace std;

class Graph {
public:
    vector<vector<pair<int,int>>> adj;
    int n; // number of nodes

    Graph(int nodes = 0) {
        n = nodes;
        adj.assign(n, {});
    }

    void resize(int new_n) {
        if (new_n <= n) return;
        adj.resize(new_n);
        n = new_n;
    }

    void add_node() {
        adj.emplace_back();
        n = (int)adj.size();
    }
        void add_edge(int u, int v, int w) {// add edge u-v with weight w
        ensure_node(u);
        ensure_node(v);
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

    void ensure_node(int u) {
        if (u >= n) resize(u+1);// resize graph
    }


};

Graph build_sample_graph() {
    Graph G(9);
    // add weighted edges 
    G.add_edge(0,1,8);
    G.add_edge(0,2,14);
    G.add_edge(0,3,13);
    G.add_edge(0,4,12);
    G.add_edge(1,2,6);
    G.add_edge(1,4,4);
    G.add_edge(2,5,8);
    G.add_edge(2,6,28);
    G.add_edge(2,8,22);
    G.add_edge(3,5,27);
    G.add_edge(4,6,25);
    G.add_edge(5,6,7);
    G.add_edge(5,7,5);
    G.add_edge(6,7,3);
    return G;
}


int main(){

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Graph G = build_sample_graph();

}
