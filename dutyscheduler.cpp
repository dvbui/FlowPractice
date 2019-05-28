#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>
#include <iterator>
#include <map>

using namespace std;
const long long oo = 1e18;

/**
 * This piece of code is taken from https://cp-algorithms.com/graph/dinic.html
 * 0-indexing
 */
struct FlowEdge {
    int v, u;
    long long cap, flow = 0;
};

struct Dinic 
{
    static const long long flow_inf = 1e18;
public:
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    int n, m = 0;
    int s, t;
    vector<int> level, ptr;
    queue<int> q;

	Dinic()
	{
		
	}
    
    Dinic(int n, int s, int t){
		this->n = n;
		this->s = s;
		this->t = t;
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int v, int u, long long cap) {
        edges.push_back(FlowEdge{v, u, cap,0});
        edges.push_back(FlowEdge{u, v, 0, 0});
        adj[v].push_back(m);
        adj[u].push_back(m + 1);
        m += 2;
    }
    
    void resetAndChangeDays(int volunteer,int newValue)
    {
		while (!q.empty()) q.pop();
		for (auto it=edges.begin(); it!=edges.end(); it=next(it,2))
		{
			if ((*it).v==0 and 1<=(*it).u and (*it).u<=volunteer)
				(*it).cap = newValue;
			
		}
		for (auto it=edges.begin(); it!=edges.end(); it++)
			(*it).flow = 0;
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

    long long dfs(int v, long long pushed) {
        if (pushed == 0)
            return 0;
        
        if (v == t)
            return pushed;
        
        for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;
            if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
                continue;
            long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0)
                continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long flow() {
        long long f = 0;
        while (true) {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs())
                break;
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, flow_inf)) {
                f += pushed;
            }
        }
        return f;
    }
    
    
    map<int,map<int,int>> getResult(int volunteer)
    {
		map<int,map<int,int>> res;
		for (auto it=edges.begin(); it!=edges.end(); it+=2)
		{
			if (1<=(*it).v and (*it).v<=volunteer)
				res[(*it).v][(*it).u-volunteer] = (*it).flow;
		}
		return res;
	}
} G;

const int maxm = 60;
const int maxn = 31;
int m,n;
string name[maxm+1];
vector<int> days[maxm+1];

void initFlow()
{
	G = Dinic(2+m+n,0,m+n+1);
	for (int i=1; i<=m; i++)
		G.add_edge(0,i,oo);
	for (int i=1; i<=m; i++)
	for (auto it=days[i].begin(); it!=days[i].end(); it++)
		G.add_edge(i,m+(*it),1);
	for (int j=m+1; j<=m+n; j++)
		G.add_edge(j,m+n+1,2);
}

bool check(int x)
{
	G.resetAndChangeDays(m,x);
	int res = G.flow();
	//cout << x << ' ' << res << '\n';
	return (res==2*n);
}
int main()
{
	ios_base::sync_with_stdio(0);
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);
	cin >> m >> n;
	for (int i=1; i<=m; i++)
	{
		int d;
		cin >> name[i] >> d;
		for (int j=1; j<=d; j++)
		{
			int k;
			cin >> k;
			days[i].push_back(k);
		}	
	}
	
	initFlow();
	
	int dau=0,cuoi=n;
	do
	{
		int giua=(dau+cuoi)/2;
		if (check(giua)) cuoi=giua-1;
		else dau=giua+1;
	}
	while (dau<=cuoi);
	
	cout << dau << '\n';
	G.resetAndChangeDays(m,dau);
	G.flow();
	map<int,map<int,int>> res=G.getResult(m);
	for (int i=1; i<=n; i++)
	{
		cout << "Day " << i << ": ";
		for (int j=1; j<=m; j++)
			if (res[j][i]) cout << name[j] << ' ';
		cout << '\n';
	}
	return 0;
}
