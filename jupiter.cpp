#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>
#include <iterator>
#include <map>

#define int long long
using namespace std;
const int oo = 1e18;

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
                f += pushed;
            }
        }
        return f;
    }
} G;


const int maxn = 30;
const int maxq = 30;
const int maxs = 100;

int N,Q,S;
int q[maxs+1];
int c[maxq+1];
int d[maxn+1],a[maxn+1][maxs+1];


main()
{
	ios_base::sync_with_stdio(0);
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);
	
	cin >> N >> Q >> S;
	for (int i=1; i<=S; i++)
		cin >> q[i];
	for (int i=1; i<=Q; i++)
		cin >> c[i];
	int sigma = 0;
	for (int i=1; i<=N; i++)
	{
		cin >> d[i];
		for (int j=1; j<=S; j++)
		{
			cin >> a[i][j];
			sigma+=a[i][j];
		}
	}
	
	G = Dinic(1+Q*N*2+N+1,0,1+Q*N*2+N);
	
	for (int i=1; i<=Q; i++)
		for (int j=1; j<=N; j++)
		{
			G.add_edge(N*2*(i-1)+j*2-1,N*2*(i-1)+j*2,c[i]); // each day, a queue can transfer at most c[i] data to the queue at the end of the day
			if (j<N) G.add_edge(N*2*(i-1)+j*2,N*2*(i-1)+j*2+1,c[i]); // and c[i] data to the next day
			G.add_edge(N*2*(i-1)+j*2,Q*N*2+j,d[j]); // each day, a queue can transfer at most d[j] data to the sink
			//cout << N*2*(i-1)+j*2 << '\n';
		}
	
	for (int i=1; i<=N; i++)
		G.add_edge(Q*N*2+i,Q*N*2+N+1,d[i]); // add wires to the sink
		
	for (int i=1; i<=N; i++)
	{
		for (int j=1; j<=S; j++)
			G.add_edge(0,N*2*(q[j]-1)+i*2-1,a[i][j]);
	}
	
	if (G.flow()==sigma)
		cout << "possible";
	else
		cout << "impossible";
	
	return 0;
}

