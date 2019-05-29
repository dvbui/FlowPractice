#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>

#define int long long
using namespace std;

/**
 * This piece of code is taken from https://cp-algorithms.com/graph/dinic.html
 * 0-indexing
 */
struct FlowEdge {
    int v, u;
    int cap, flow = 0;
    FlowEdge(int v, int u, int cap) : v(v), u(u), cap(cap) {}
};

struct Dinic {
    static const int flow_inf = 1e18;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    int n, m = 0;
    int s, t;
    vector<int> level, ptr;
    queue<int> q;
public:
	Dinic()
	{
		
	}
    Dinic(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int v, int u, int cap) {
		//cout << v << ' ' << u << ' ' << cap << '\n';
        edges.emplace_back(v, u, cap);
        edges.emplace_back(u, v, 0);
        adj[v].push_back(m);
        adj[u].push_back(m + 1);
        m += 2;
    }

    bool bfs() {
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap - edges[id].flow < 1)
                    continue;
                if (level[edges[id].u] != -1)
                    continue;
                level[edges[id].u] = level[v] + 1;
                q.push(edges[id].u);
            }
        }
        return level[t] != -1;
    }

    int dfs(int v, int pushed) {
        if (pushed == 0)
            return 0;
        if (v == t)
            return pushed;
        for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;
            if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
                continue;
            int tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0)
                continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }
    
    void resetFlow()
    {
		for (auto it=edges.begin(); it!=edges.end(); it++)
			(*it).flow = 0;
		while (!q.empty()) q.pop();		
	}
			

    int flow() {
		int f = 0;
        while (true) {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs())
                break;
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, flow_inf)) {
				f+=pushed;
            }
        }
        return f;
        
    }
} G;
const int maxn = 1000;
const int maxm = 2000;
int n,m;
int b[maxn+1],e[maxn+1];

bool is_weekend(int x)
{
	return (x%7==0 or x%7==6);
}

void solve()
{
	cin >> n >> m;
	m/=2;
	for (int i=1; i<=n; i++)
		cin >> b[i] >> e[i];
	
	G = Dinic(1+n+100+1,0,1+n+100);
	
	for (int i=1; i<=n; i++)
		G.add_edge(0,i,1);
		
	for (int i=1; i<=n; i++)
	{
		for (int j=b[i]; j<=e[i]; j++)
			if (!is_weekend(j)) G.add_edge(i,n+j,1);
	}
	
	for (int i=n+1; i<=n+100; i++)
		G.add_edge(i,1+n+100,m);
	
	if (G.flow()==n)
	{
		cout << "fine\n";
		return;
	}
	
	for (int i=1; i<=n; i++)
	{
		for (int j=b[i]; j<=e[i]; j++)
			if (is_weekend(j)) G.add_edge(i,n+j,1);
	}
	
	G.resetFlow();
	
	if (G.flow()==n)
	{
		cout << "weekend work\n";
	}
	else
		cout << "serious trouble\n";
}
main()
{
	ios_base::sync_with_stdio(0);
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);
	int t;
	cin >> t;
	for (int i=1; i<=t; i++)
		solve();
	
	return 0;
}
