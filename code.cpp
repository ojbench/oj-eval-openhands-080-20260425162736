
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

const int MAXN = 100005;
vector<int> adj[MAXN];
int dfn[MAXN], low[MAXN], timer;
stack<pair<int, int>> edge_stack;
bool in_odd_cycle[MAXN];
int color[MAXN];
vector<int> component_nodes;
vector<pair<int, int>> component_edges;

bool is_bipartite(int u_local_idx, int c, const vector<int>& nodes, const vector<vector<int>>& local_adj) {
    int u = nodes[u_local_idx];
    color[u] = c;
    for (int v_local_idx : local_adj[u_local_idx]) {
        int v = nodes[v_local_idx];
        if (color[v] == -1) {
            if (!is_bipartite(v_local_idx, 1 - c, nodes, local_adj)) return false;
        } else if (color[v] == c) {
            return false;
        }
    }
    return true;
}

void process_component(const vector<pair<int, int>>& edges) {
    if (edges.empty()) return;
    vector<int> nodes;
    for (auto& e : edges) {
        nodes.push_back(e.first);
        nodes.push_back(e.second);
    }
    sort(nodes.begin(), nodes.end());
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

    int n = nodes.size();
    vector<vector<int>> local_adj(n);
    for (auto& e : edges) {
        int u = lower_bound(nodes.begin(), nodes.end(), e.first) - nodes.begin();
        int v = lower_bound(nodes.begin(), nodes.end(), e.second) - nodes.begin();
        local_adj[u].push_back(v);
        local_adj[v].push_back(u);
    }

    for (int node : nodes) color[node] = -1;
    
    bool bipartite = true;
    // Since it's a biconnected component, it's connected.
    if (!is_bipartite(0, 0, nodes, local_adj)) {
        bipartite = false;
    }

    if (!bipartite) {
        for (int node : nodes) {
            in_odd_cycle[node] = true;
        }
    }
}

void find_bcc(int u, int p = -1) {
    dfn[u] = low[u] = ++timer;
    for (int v : adj[u]) {
        if (v == p) continue;
        if (dfn[v] < dfn[u]) {
            if (dfn[v] == 0) {
                edge_stack.push({u, v});
                find_bcc(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    vector<pair<int, int>> edges;
                    while (true) {
                        pair<int, int> edge = edge_stack.top();
                        edge_stack.pop();
                        edges.push_back(edge);
                        if (edge.first == u && edge.second == v) break;
                    }
                    process_component(edges);
                }
            } else {
                low[u] = min(low[u], dfn[v]);
                edge_stack.push({u, v});
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        if (u == v) continue;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= n; ++i) {
        if (dfn[i] == 0) {
            find_bcc(i);
        }
    }

    int count = 0;
    for (int i = 1; i <= n; ++i) {
        if (!in_odd_cycle[i]) {
            count++;
        }
    }

    cout << count << endl;

    return 0;
}
