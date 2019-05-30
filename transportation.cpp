#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cstring>
using namespace std;
const int maxs = 600;
const int maxt = 1000;
const int oo = (int) 1e9;
const int MAXN = 1200+2000+2;
int s,r,f,t;
map<string,int> stateName;
vector<int> V[maxt+1];

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
	
	void addEdge(int u,int v,int capacity,string note = "")
	{
		//cout << u << ' ' << v << ' ' << capacity << ' ' << note << '\n';
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

MaxFlow G;

int main()
{
	ios_base::sync_with_stdio(0);
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);
	cin >> s >> r >> f >> t;
	
	for (int i=1; i<=r; i++)
	{
		string s;
		cin >> s;
		stateName[s] = i;
	}
	
	
	for (int i=1; i<=f; i++)
	{
		string s;
		cin >> s;
		stateName[s] = r+i;
	}
	
	for (int i=1; i<=t; i++)
	{
		int n;
		cin >> n;
		for (int j=1; j<=n; j++)
		{
			string s;
			cin >> s;
			if (stateName.count(s))
				V[i].push_back(stateName[s]);
			else
			{
				stateName[s] = 0;
				stateName[s] = stateName.size();
				V[i].push_back(stateName.size());
			}
		}
		sort(V[i].begin(), V[i].end());
	}
		
	//G = MaxFlow(1+2*s+2*t+1,1,2*s+2*t+2);
	
	// source: 1
	// index for state (in) : 2 -> s
	// index for state (out) : 1+s+1 -> 1+2*s
	// index for company (in) : 1+2*s+1 -> 1+2*s+t
	// index for company (out) : 1+2*s+t+1 -> 1+2*s+2*t
	// sink : 2*s+2*t+2
	
	HLPP<MAXN> G;
	G.s = 0;
	G.t = 2*s+2*t+1;
	
	for (int i=1; i<=r+f; i++)
		G.addEdge(i,s+i,1,"between state");
		
	for (int i=1; i<=t; i++)
		G.addEdge(2*s+i,2*s+t+i,1,"between company");
		
	
	for (int i=1; i<=r; i++) // raw material
		G.addEdge(0,i,1,"source to mat");
	
	for (int i=1; i<=f; i++) // factory
		G.addEdge(s+r+i,1+2*s+2*t,1,"factory to sink");
	
	for (int i=1; i<=t; i++)
	{
		for (auto it=V[i].begin(); it!=V[i].end(); it++)
		{
			if ((*it)<=r)
				G.addEdge(s+(*it),2*s+i,1,"mat to company");
			else
			{
				G.addEdge(2*s+t+i,(*it),1,"company to factory or free state");
				
				if ((*it)>r+f)
					G.addEdge((*it),2*s+i,1,"free state to company");
			}
		}
	}
	
	cout << G.calc();
		
	
	return 0;
}
	
