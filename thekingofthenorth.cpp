#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#define int long long
using namespace std;
const int maxn = 300;
int n,m;
int x,y;
int a[maxn+1][maxn+1];
bool visit[maxn+1][maxn+1];
int mx[4] = {0,0,1,-1};
int my[4] = {1,-1,0,0};

template <int MAXN, class T = int> struct HLPP {
    const T INF = numeric_limits<T>::max();
    struct edge {
        int to, rev;
        T f;
    };
    int s = MAXN - 1, t = MAXN - 2;
    vector<edge> adj[MAXN];
    vector<int> lst[MAXN], gap[MAXN];
    T excess[MAXN];
    int highest, height[MAXN], cnt[MAXN], work;
    void addEdge(int from, int to, int f, string note = "", bool isDirected = true) {
		//cout << from << ' ' << to << ' ' << f << '\n';
        adj[from].push_back({to, adj[to].size(), f});
        adj[to].push_back({from, adj[from].size() - 1, isDirected ? 0 : f});
    }
    void updHeight(int v, int nh) {
        work++;
        if (height[v] != MAXN)
            cnt[height[v]]--;
        height[v] = nh;
        if (nh == MAXN)
            return;
        cnt[nh]++, highest = nh;
        gap[nh].push_back(v);
        if (excess[v] > 0)
            lst[nh].push_back(v);
    }
    void globalRelabel() {
        work = 0;
        for (int i=0; i<MAXN; i++)
			height[i] = MAXN;
        memset(cnt,0,sizeof(cnt));
        for (int i = 0; i < highest; i++)
            lst[i].clear(), gap[i].clear();
        height[t] = 0;
        queue<int> q({t});
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto &e : adj[v])
                if (height[e.to] == MAXN && adj[e.to][e.rev].f > 0)
                    q.push(e.to), updHeight(e.to, height[v] + 1);
            highest = height[v];
        }
    }
    void push(int v, edge &e) {
        if (excess[e.to] == 0)
            lst[height[e.to]].push_back(e.to);
        T df = min(excess[v], e.f);
        e.f -= df, adj[e.to][e.rev].f += df;
        excess[v] -= df, excess[e.to] += df;
    }
    void discharge(int v) {
        int nh = MAXN;
        for (auto &e : adj[v]) {
            if (e.f > 0) {
                if (height[v] == height[e.to] + 1) {
                    push(v, e);
                    if (excess[v] <= 0)
                        return;
                } else
                    nh = min(nh, height[e.to] + 1);
            }
        }
        if (cnt[height[v]] > 1)
            updHeight(v, nh);
        else {
            for (int i = height[v]; i <= highest; i++) {
                for (auto j : gap[i])
                    updHeight(j, MAXN);
                gap[i].clear();
            }
        }
    }
    T calc(int heur_n = MAXN) {
        memset(excess,0,sizeof(excess));
        excess[s] = INF, excess[t] = -INF;
        globalRelabel();
        for (auto &e : adj[s])
            push(s, e);
        for (; highest >= 0; highest--) {
            while (!lst[highest].empty()) {
                int v = lst[highest].back();
                lst[highest].pop_back();
                discharge(v);
                if (work > 4 * heur_n)
                    globalRelabel();
            }
        }
        return excess[t] + INF;
    }
};

HLPP<2*maxn*maxn+2> G;

int index(int x,int y)
{
	return (x-1)*m+y;
}

bool ok(int x,int y)
{
	return (1<=x and x<=n and 1<=y and y<=m);
}

void dfs(int x,int y)
{
	visit[x][y] = true;
	for (int i=0; i<4; i++)
	{
		int nx=x+mx[i];
		int ny=y+my[i];
		if (!ok(nx,ny))
			G.addEdge(index(x,y)+n*m,2*n*m+1,G.INF);
		else
		if (a[nx][ny]!=0)
			G.addEdge(index(x,y)+n*m,index(nx,ny),G.INF);
			
		if (ok(nx,ny) and a[nx][ny]!=0 and !visit[nx][ny])
			dfs(nx,ny);		
	}
	
	
}


main()
{
	ios_base::sync_with_stdio(0);
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);
	cin >> n >> m;
	
	for (int i=1; i<=n; i++)
		for (int j=1; j<=m; j++)
		{
			cin >> a[i][j];
			G.addEdge(index(i,j),index(i,j)+n*m,a[i][j]);
		}
	
	cin >> x >> y;
	x++;
	y++;
	
	G.s = 0;
	G.t = 2*n*m+1;
	
	G.addEdge(0,index(x,y),G.INF);
	
	dfs(x,y);
	
	cout << G.calc();
	
	
	return 0;
}
