#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>
using namespace std;
const int maxn = 200;
const int maxm = 20;
const int oo = (int) 1e9;
int n,m,k;
int sumr[maxn+1],sumc[maxm+1];
int mini[maxn+1][maxm+1];
int maxi[maxn+1][maxm+1];
int res[maxn+1][maxm+1];
char op[maxn+1][maxm+1];

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
	map<int,map<int,int> > res;
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
	
	void createMap()
	{
		for (int i=1; i<=m; i++)
		{
			if (e[i].f>0)
				res[e[i].x][e[i].y]=e[i].f;
		}
	}
		
	 
};
MaxFlow G;

void solve()
{
	memset(res,0,sizeof(res));
	memset(op,' ',sizeof(op));
	
	cin >> n >> m;
	
	sumr[0] = 0;
	for (int i=1; i<=n; i++)
	{
		cin >> sumr[i];
	}
	
	sumc[0] = 0;
	for (int i=1; i<=m; i++)
	{
		cin >> sumc[i];
	}
	
	cin >> k;
	
	for (int i=1; i<=n; i++)
	for (int j=1; j<=m; j++)
	{
		maxi[i][j] = oo;
		mini[i][j] = 0;
	}
	
	G = MaxFlow(n+m+2,1,n+m+2);
	
	for (int i=1; i<=k; i++)
	{
		int x,y;
		int v;
		cin >> x >> y >> op[x][y] >> v;
		
		int maxx = x,minx = x;
		int maxy = y,miny = y;
		
		if (x==0)
		{
			minx = 1;
			maxx = n;
		}
		
		if (y==0)
		{
			miny = 1;
			maxy = m;
		}
		
		for (int xx=minx; xx<=maxx; xx++)
		for (int yy=miny; yy<=maxy; yy++)
		{
			if (op[x][y]=='=')
			{
				mini[xx][yy] = max(mini[xx][yy],v);
				maxi[xx][yy] = min(maxi[xx][yy],v);
			}
			else
			if (op[x][y]=='>')
				mini[xx][yy] = max(mini[xx][yy],v+1);
			else
				maxi[xx][yy] = min(maxi[xx][yy],v-1);
		}
	}
	
	for (int i=1; i<=n; i++)
	for (int j=1; j<=m; j++)
	{
		
		if (mini[i][j]>maxi[i][j])
		{
			cout << "IMPOSSIBLE";
			return;
		}
		
		sumr[i]-=mini[i][j];
		sumc[j]-=mini[i][j];
		res[i][j]=mini[i][j];
		maxi[i][j]-=mini[i][j];
		G.addEdge(1+i,1+n+j,maxi[i][j]);
	}
	
	for (int i=1; i<=n; i++)
		G.addEdge(1,1+i,sumr[i]);
	
	for (int j=1; j<=m; j++)
		G.addEdge(1+n+j,n+m+2,sumc[j]);
		
	for (int i=1; i<=n; i++)
		sumr[0]+=sumr[i];
	for (int j=1; j<=m; j++)
		sumc[0]+=sumc[j];
	
	
	if (sumr[0]==sumc[0] and G.getResult()==sumr[0])
	{
		G.createMap();
		
		for (int i=1; i<=n; i++)
		for (int j=1; j<=m; j++)
			res[i][j]+=G.res[1+i][1+n+j];
			
		for (int i=1; i<=n; i++)
		{
			for (int j=1; j<=m; j++)
				cout << res[i][j] << ' ';
			if (i!=n) cout << '\n';
		}
	}
	else
		cout << "IMPOSSIBLE";
}
int main()
{
	ios_base::sync_with_stdio(0);
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);
	int t;
	cin >> t;
	for (int i=1; i<=t; i++)
	{
		solve();
		cout << '\n';
		if (i!=t) cout << '\n';
	}
	return 0;
}
