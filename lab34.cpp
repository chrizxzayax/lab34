// // comsc 210 | lab33 | Christian Molina
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <string>

#include <iomanip>



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

       bool valid(int u) const {
        return u >= 0 && u < n;
    }

    // BFS from start: returns order visited
    vector<int> bfs(int start) const {
        vector<int> order;
        if (!valid(start)) return order;
        vector<char> vis(n,0);
        queue<int> q;
        vis[start]=1; q.push(start);
        while(!q.empty()){
            int u=q.front(); q.pop();
            order.push_back(u);
            for (auto &p: adj[u]) {
                int v = p.first;
                if (!vis[v]) { vis[v]=1; q.push(v); }
            }
        }
        return order;
    }

};

void print_network_description(const Graph &g) {
    cout << "Water Distribution Network Topology (simple view):\n";
    for (int i = 0; i < g.n; ++i) {
        cout << " Junction " << setw(2) << i << " connected to: ";
        if (g.adj[i].empty()) { cout << "(none)"; }
        else {
            for (auto &p: g.adj[i]) {
                cout << p.first << "(" << p.second << ")\t";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

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
int prompt_int(const string &msg) {
    cout << msg;
    int v; while(!(cin >> v)) { cin.clear(); cin.ignore(1024,'\n'); cout << "Invalid. " << msg; }
    return v;
}

// Utility: read string line
string read_line_trim() {
    string s;
    getline(cin, s);
    if (!s.empty() && s.back()=='\r') s.pop_back();
    return s;
}


int main(){

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Graph G = build_sample_graph();
    cout << "Sample graph loaded (nodes: " << G.n << ").\n\n";

    bool running = true;

    while(running){

        menu_print();
    }

}
