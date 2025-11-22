// // comsc 210 | lab33 | Christian Molina
#include <iostream>
#include <tuple>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <set>

const int INF = std::numeric_limits<int>::max();
using namespace std;

class Graph {
public:
    vector<vector<pair<int,int>>> adj;
    int n; 

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

    void add_edge(int u, int v, int w) {
        ensure_node(u);
        ensure_node(v);
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }
    void remove_edge(int u, int v) {
        if (!valid(u) || !valid(v)) return;
        auto &A = adj[u];
        A.erase(remove_if(A.begin(), A.end(), [&](const pair<int,int>&p){ return p.first==v; }), A.end());
        auto &B = adj[v];
        B.erase(remove_if(B.begin(), B.end(), [&](const pair<int,int>&p){ return p.first==u; }), B.end());
    }

    void remove_node(int u) {
        if (!valid(u)) return;
        // remove edges 
        for (int i = 0; i < n; ++i) {
            if (i==u) continue;
            remove_edge(i,u);
        }
        adj[u].clear();
    }

    bool change_weight(int u, int v, int neww) {
        if (!valid(u) || !valid(v)) return false;
        bool found=false;
        for (auto &p: adj[u]) if (p.first==v) { p.second=neww; found=true; }
        for (auto &p: adj[v]) if (p.first==u) { p.second=neww; found=true; }
        return found;
    }

    void print_adj() const {
        cout << "Graph adjacency list:\n";
        for (int i = 0; i < n; ++i) {
            cout << i << " -> ";
            for (auto &p: adj[i]) {
                cout << "(" << p.first << ", " << p.second << ") ";
            }
            cout << "\n";
        }
    }

    void ensure_node(int u) {
        if (u >= n) resize(u+1);
    }

    bool valid(int u) const {
        return u >= 0 && u < n;
    }

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

    vector<int> dfs(int start) const {
        vector<int> order;
        if (!valid(start)) return order;
        vector<char> vis(n,0);
        stack<int> st;
        st.push(start);
        while(!st.empty()){
            int u = st.top(); st.pop();
            if (vis[u]) continue;
            vis[u]=1;
            order.push_back(u);
            vector<int> neigh;
            for (auto &p: adj[u]) neigh.push_back(p.first);
            sort(neigh.begin(), neigh.end(), greater<int>()); 
            for (int v : neigh) if (!vis[v]) st.push(v);
        }
        return order;
    }

    vector<long long> dijkstra(int src) const {
        vector<long long> dist(n, (long long)INF);
        if (!valid(src)) return dist;
        using pli = pair<long long,int>;
        priority_queue<pli, vector<pli>, greater<pli>> pq;
        dist[src]=0;
        pq.push({0,src});
        while(!pq.empty()){
            auto [d,u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            for (auto &p: adj[u]) {
                int v = p.first; int w = p.second;
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

    pair<long long, vector<tuple<int,int,int>>> prim_mst() const {
        vector<char> used(n, 0);
        vector<long long> mincost(n, LLONG_MAX);
        vector<int> parent(n, -1);
        for (int i=0;i<n;i++) {
            if (!adj[i].empty()) { mincost[i]=0; parent[i]=-1; break; }
            if (i==n-1) { // graph empty: nothing to do
                vector<tuple<int,int,int>> emp; return {0, emp};
            }
        }
        using pli = pair<long long,int>;
        priority_queue<pli, vector<pli>, greater<pli>> pq;
        // find initial node
        int start = 0;
        for (int i=0;i<n;++i) { if (!adj[i].empty()) { start=i; break; } }
        fill(mincost.begin(), mincost.end(), LLONG_MAX);
        mincost[start]=0;
        pq.push({0,start});
        long long total = 0;
        vector<tuple<int,int,int>> edges;
        while(!pq.empty()){
            auto [c,u] = pq.top(); pq.pop();
            if (used[u]) continue;
            used[u]=1;
            total += c;
            if (parent[u] != -1) edges.emplace_back(parent[u], u, (int)c);
            for (auto &p: adj[u]) {
                int v = p.first; int w = p.second;
                if (!used[v] && w < mincost[v]) {
                    mincost[v] = w;
                    parent[v] = u;
                    pq.push({mincost[v], v});
                }
            }
        }
        return {total, edges};
    }

    vector<int> dijkstra_parent_path(int src, int dest, vector<int> &parent_out) const {
        parent_out.assign(n, -1);
        vector<long long> dist(n, LLONG_MAX);
        using pli = pair<long long,int>;
        priority_queue<pli, vector<pli>, greater<pli>> pq;
        dist[src]=0;
        pq.push({0,src});
        while(!pq.empty()){
            auto [d,u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            for (auto &p: adj[u]) {
                int v=p.first; int w=p.second;
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    parent_out[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
        // build path
        vector<int> path;
        if (dist[dest] == LLONG_MAX) return path;
        for (int cur = dest; cur != -1; cur = parent_out[cur]) path.push_back(cur);
        reverse(path.begin(), path.end());
        return path;
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
string read_line_trim() {
    string s;
    getline(cin, s);
    if (!s.empty() && s.back()=='\r') s.pop_back();
    return s;
}
void print_dijkstra_result(const Graph &G, int src) {
    if (!G.valid(src)) { cout << "Invalid source\n"; return; }
    auto dist = G.dijkstra(src);
    cout << "Shortest path distances from node " << src << ":\n";
    for (int i = 0; i < G.n; ++i) {
        if (dist[i] >= INF) cout << src << " -> " << i << " : unreachable\n";
        else cout << src << " -> " << i << " : " << dist[i] << "\n";
    }
}

void print_mst(const Graph &G) {
    auto res = G.prim_mst();
    long long total = res.first;
    auto edges = res.second;
    cout << "Minimum Spanning Tree edges (Prim): total weight = " << total << "\n";
    for (auto &t: edges) {
        int u,v,w; tie(u,v,w)=t;
        cout << " Edge from " << u << " to " << v << " with capacity: " << w << " units\n";
    }
    cout << "\n";
}

void menu_print() {
    cout << "Water Distribution Network Menu:\n";
    cout << "[1] Display network adjacency list\n";
    cout << "[2] Check contaminant spread (BFS)\n";
    cout << "[3] Plan inspection route (DFS)\n";
    cout << "[4] Calculate shortest paths (Dijkstra)\n";
    cout << "[5] Find Minimum Spanning Tree (Prim)\n";
    cout << "[6] Modify graph (add/remove node/edge/change weight)\n";
    cout << "[7] Print domain description (junctions view)\n";
    cout << "[0] Exit\n";
    cout << "Enter your choice: ";
}

int main(){

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Graph G = build_sample_graph();
    cout << "Sample graph loaded (nodes: " << G.n << ").\n\n";

    bool running = true;

    while(running){

        menu_print();
        int choice;
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1024,'\n');
            cout << "Invalid input\n"; continue;// continue loop
        }
        cin.ignore(1024,'\n'); // flush eol
        switch(choice) {
            case 1: {
                G.print_adj();
                break;
            }
            case 2: {
                int start = prompt_int("BFS start node: ");
                auto order = G.bfs(start);
                cout << "BFS order starting from vertex " << start << ":\n";
                for (int v: order) cout << v << " ";
                cout << "\n";
                break;
            }
            case 3: {
                int start = prompt_int("DFS start node: ");
                auto order = G.dfs(start);
                cout << "DFS order starting from vertex " << start << ":\n";
                for (int v: order) cout << v << " ";
                cout << "\n";
                break;
            }
            case 4: {
                int src = prompt_int("Dijkstra source node: ");
                print_dijkstra_result(G, src);
                break;
            }
            case 5: {
                print_mst(G);
                break;
            }
            case 6: {
                cout << "Graph modification menu:\n";
                cout << "[1] Add node\n";
                cout << "[2] Remove node (clears adjacency)\n";
                cout << "[3] Add undirected edge\n";
                cout << "[4] Remove edge\n";
                cout << "[5] Change edge weight\n";
                cout << "Choice: ";
                int m; if(!(cin>>m)) { cin.clear(); cin.ignore(1024,'\n'); cout<<"Invalid\n"; break; }
                if (m==1) {
                    G.add_node();
                    cout << "Added node " << (G.n-1) << "\n";
                } else if (m==2) {
                    int u = prompt_int("Node to remove: ");
                    G.remove_node(u);
                    cout << "Removed node (cleared) " << u << "\n";
                } else if (m==3) {
                    int u = prompt_int("u: "); int v = prompt_int("v: "); int w = prompt_int("weight: ");
                    G.add_edge(u,v,w);
                    cout << "Added edge " << u << " <-> " << v << " w=" << w << "\n";
                } else if (m==4) {
                    int u = prompt_int("u: "); int v = prompt_int("v: ");
                    G.remove_edge(u,v);
                    cout << "Removed edge(s) between " << u << " and " << v << "\n";
                } else if (m==5) {
                    int u = prompt_int("u: "); int v = prompt_int("v: "); int w = prompt_int("new weight: ");
                    if (G.change_weight(u,v,w)) cout << "Weight changed\n"; else cout << "Edge not found\n";
                } else cout << "Unknown choice\n";
                break;
            }
            case 7: {
                print_network_description(G);
                break;
            }
            case 0: {
                running = false;
                break;
            }
            default:
                cout << "Invalid menu choice\n";
        }
        cout << "\n";
    }

    cout << "Exiting.\n";

    return 0;
}
