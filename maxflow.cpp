#include <iostream>
#include <cstdio>
#include <queue>
#include <unordered_map>
#include <cstring>
#define map unordered_map
using namespace std;

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
	map<int,TEdge> e;
	map<int,int> link;
	int head[maxn+1],current[maxn+1];
	int excess[maxn+1];
	int h[maxn+1];
	int count[maxn*2];
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
		
		memset(head,0,sizeof(head));
		
		e.reserve(1024);
		e.max_load_factor(0.25);
		link.reserve(1024);
		link.max_load_factor(0.25);
	}
	
	void addEdge(int u,int v,int capacity)
	{
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
		
		memset(excess,0,sizeof(excess));
		
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
		memset(count,0,sizeof(count));
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
	
	void printResult()
	{
		init();
		FIFOPreflowPush();
		cout << n << ' ' << flowValue << ' ';
		int cnt = 0;
		for (int i=1; i<=m; i++)
		{
			if (e[i].f>0)
				cnt++;
		}
		
		cout << cnt << '\n';
		for (int i=1; i<=m; i++)
		{
			if (e[i].f>0)
				cout << e[i].x-1 << ' ' << e[i].y-1 << ' ' << e[i].f << '\n';
		}
	}
	 
};
MaxFlow G;

int main()
{
	ios_base::sync_with_stdio(0);
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);
	
	int n,m,s,t;
	cin >> n >> m >> s >> t;
	s++;
	t++;
	
	G = MaxFlow(n,s,t);
	
	for (int i=1; i<=m; i++)
	{
		int u,v,cap;
		cin >> u >> v >> cap;
		u++;
		v++;
		G.addEdge(u,v,cap);
	}
	
	G.printResult();
	
	return 0;
}
