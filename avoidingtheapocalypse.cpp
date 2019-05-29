#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <unordered_map>
#include <cstring>
#define ii pair<int,int>
#define X first
#define Y second
using namespace std;
const int maxn = 1000;
const int maxt = 100;
const int oo = (int) 1e9;
int n;
int initial,g,s;
int m;
int x[maxn+1];
int r;
struct edge
{
	int v,p,t;
};
vector<edge> V[maxn+1];
map<ii,int> M;
set<ii> visited;

struct TEdge
{
	int x,y,c,f;
};

/**
 * Benchmark: 0.1889s in max case
 * Adapted from the textbook
 * 1-indexing
 */
class MaxFlow
{
	static const int maxn = 1000;
	static const int maxm = 100000;
	int n,m,s,t,flowValue;
	unordered_map<int,TEdge> e;
	unordered_map<int,int> link;
	vector<int> head,current,excess,h,count;
	queue<int> Q;
public:
	MaxFlow()
	{
		
	}
	
	MaxFlow(int n,int s,int t)
	{
		this->n = n;
		this->m = 0;
		this->s = s;
		this->t = t;
		
		head.resize(n+1);
		current.resize(n+1);
		excess.resize(n+1);
		h.resize(n+1);
		count.resize(2*n);
		
		e.reserve(1024);
		e.max_load_factor(0.25);
		link.reserve(1024);
		link.max_load_factor(0.25);
	}
	
	void addEdge(int u,int v,int capacity)
	{
		//cout << u << ' ' << v << ' ' << capacity << '\n';
		m++;
		e[m].x = u;
		e[m].y = v;
		e[m].c = capacity;
		link[m] = head[u];
		head[u] = m;
		
		e[-m].x = v;
		e[-m].y = u;
		e[-m].c = 0;
		link[-m] = head[v];
		head[v] = -m;
	}
	
	void init()
	{
		for (int v=1; v<=n; v++)
			current[v] = head[v];
			
		for (int i=-m; i<=m; i++)
			e[i].f = 0;
		
		for (auto it=excess.begin(); it!=excess.end(); it++)
			(*it) = 0;
		
		int i=head[s];
		while (i!=0)
		{
			int sf = e[i].c;
			e[i].f = sf;
			e[-i].f = -sf;
			excess[e[i].y]+=sf;
			excess[s]-=sf;
			i = link[i];
		}
		
		for (int v=1; v<=n; v++)
			h[v] = 1;
		
		h[s] = n;
		h[t] = 0;
		for (auto it=count.begin(); it!=count.end(); it++)
			(*it) = 0;
			
		count[n] = 1;
		count[0] = 1;
		count[1] = n-2;
		
		while (!Q.empty()) Q.pop();
		
		for (int v=1; v<=n; v++)
		{
			if (v!=s and v!=t and excess[v]>0)
				Q.push(v);
		}
	}
	
	void push(int i)
	{
		int delta;
		delta=min(excess[e[i].x],e[i].c-e[i].f);
		e[i].f+=delta;
		e[-i].f-=delta;
		excess[e[i].x]-=delta;
		excess[e[i].y]+=delta;
	}
	
	void setH(int u,int newH)
	{
		count[h[u]]--;
		h[u] = newH;
		count[h[u]]++;
	}
	
	void performGapHeurisitic(int gap)
	{
		if (0<gap and gap<n and count[gap]==0)
		{
			for (int v=1; v<=n; v++)
			{
				if (v!=s and gap<h[v] and h[v]<=n)
				{
					setH(v,n+1);
					current[v] = head[v];
				}
			}
		}
	}
	
	void lift(int u)
	{
		int minH = 2*maxn;
		int i=head[u];
		while (i!=0)
		{
			if (e[i].c>e[i].f and h[e[i].y]<minH)
				minH=h[e[i].y];
			i = link[i];
		}
		
		int oldH = h[u];
		setH(u,minH + 1);
		performGapHeurisitic(oldH);
	}
	
	void FIFOPreflowPush()
	{
		while (!Q.empty())
		{
			int z=Q.front();
			Q.pop();
			while (current[z]!=0)
			{
				if (e[current[z]].c>e[current[z]].f and h[e[current[z]].x]>h[e[current[z]].y])
				{
					bool needQueue = (e[current[z]].y!=s) and (e[current[z]].y!=t) and (excess[e[current[z]].y]==0);
					push(current[z]);
					if (needQueue)
						Q.push(e[current[z]].y);
					if (excess[z]==0) break;
				}
				current[z] = link[current[z]];
			}
			
			if (excess[z]>0)
			{
				lift(z);
				current[z] = head[z];
				Q.push(z);
			}
		}
		
		flowValue = excess[t];
	}
	
	int getResult()
	{
		init();
		FIFOPreflowPush();
		
		/*for (int i=1; i<=m; i++)
		{
			if (e[i].f>0)
				cout << e[i].x << ' ' << e[i].y << ' ' << e[i].c << ' ' << e[i].f << '\n';
			
		}*/
		return flowValue;
	}
	 
};
MaxFlow G;


void dfs(int x,int y)
{
	M[ii(x,y)] = 0;
	if (y<s and !M.count(ii(x,y+1))) dfs(x,y+1);
	for (auto it=V[x].begin(); it!=V[x].end(); it++)
	{
		if (y+(*it).t<=s and !M.count(ii((*it).v,y+(*it).t)))
			dfs((*it).v,y+(*it).t);
	}
}

void createFlow(int x,int y)
{
	visited.insert(ii(x,y));
	if (y<s)
	{
		G.addEdge(M[ii(x,y)],M[ii(x,y+1)],oo);
		if (!visited.count(ii(x,y+1)))
			createFlow(x,y+1);
	}
	
	for (auto it=V[x].begin(); it!=V[x].end(); it++)
	{
		if (y+(*it).t<=s)
		{
			G.addEdge(M[ii(x,y)],M[ii((*it).v,y+(*it).t)],(*it).p);
			if (!visited.count(ii((*it).v,y+(*it).t)))
				createFlow((*it).v,y+(*it).t);
		}
	}
}

void solve()
{
	cin >> n >> initial >> g >> s >> m;
	for (int i=1; i<=m; i++)
		cin >> x[i];
	cin >> r;
	for (int i=1; i<=r; i++)
	{
		int a,b,p,t;
		cin >> a >> b >> p >> t;
		V[a].push_back(edge{b,p,t});
	}
	
	dfs(initial,0);
	
	int cnt = 1;
	//cout << "map\n";
	for (auto it=M.begin(); it!=M.end(); it++)
	{
		cnt++;
		(*it).second = cnt;
		//cout << (*it).X.X << ' ' << (*it).X.Y << ' ' << (*it).Y << '\n';
	}
	//cout << '\n';
	
	G = MaxFlow(2+M.size(),1,2+M.size());
	
	G.addEdge(1,M[ii(initial,0)],g);
	
	createFlow(initial,0);

	for (int i=1; i<=m; i++)
	{
		for (int j=0; j<=s; j++)
			if (M.count(ii(x[i],j)))
				G.addEdge(M[ii(x[i],j)],2+M.size(),oo);
	}
	
	cout << G.getResult() << '\n';
	
	for (int i=1; i<=n; i++)
		V[i].clear();
	M.clear();
	visited.clear();
}
int main()
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
