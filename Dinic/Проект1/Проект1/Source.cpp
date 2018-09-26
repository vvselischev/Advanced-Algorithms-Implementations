#include<iostream>
#include<conio.h>
#include<vector>
#include<algorithm>
#include<queue>

using namespace std;

struct Edge
{
	int a, b, f, c;
	int next;
};

vector<Edge> edges;
int head[501];
int originHead[501];
int last[501];
bool visited[501];
int d[501];
long long maxC;
int n, m, s, t;

bool bfs(long long start)
{
	fill(d, d + n, INT32_MAX);
	queue<long long> q;
	q.push(start);
	visited[start] = true;
	d[start] = 0;
	while (!q.empty())
	{
		long long v = q.front();
		q.pop();
		for (long long i = head[v]; i != -1; i = edges[i].next)
		{
			Edge e = edges[i];
			if (d[e.b] == INT32_MAX && e.c - e.f >= maxC)
			{
				q.push(e.b);
				d[e.b] = d[v] + 1;
			}
		}
	}
	return d[t] != INT32_MAX;
}

bool dfs(long long v)
{
	if (v == t)
	{
		return true;
	}
	while (head[v] != -1)
	{
		long long i = head[v];
		Edge e = edges[i];
		if (d[e.a] + 1 == d[e.b])
		{
			if (e.c - e.f >= maxC)
			{
				if (e.b == t || dfs(e.b))
				{
					edges[i].f += maxC;
					edges[i ^ 1].f -= maxC;
					return true;
				}
			}
		}
		head[v] = edges[i].next;
	}
	return false;
}

int main()
{
	cin >> n >> m;
	s = 0;
	t = n - 1;
	fill(head, head + n, -1);
	for (long long i = 0; i < m; i++)
	{
		int a, b, c;
		cin >> a >> b >> c;
		a--;
		b--;
		if (head[a] == -1)
		{
			head[a] = edges.size();	
		}
		else
		{
			edges[last[a]].next = edges.size();
		}
		last[a] = edges.size();
		edges.push_back({ a, b, 0, c, -1 });

		if (head[b] == -1)
		{
			head[b] = edges.size();
		}
		else
		{
			edges[last[b]].next = edges.size();
		}
		last[b] = edges.size();
		edges.push_back({ b, a, 0, 0, -1 });
	}

	for (long long i = 0; i < n; i++)
	{
		originHead[i] = head[i];
	}

	long long maxFlow = 0;
	maxC = 1 << 30;
	while (maxC > 0)
	{
		while (bfs(s))
		{
			while (dfs(s))
			{
				maxFlow += maxC;
			}
			for (long long i = 0; i < n; i++)
			{
				head[i] = originHead[i];
			}
		}
		maxC /= 2;
	}

	cout << maxFlow << '\n';
	for (Edge e : edges)
	{
		if (e.c != 0)
		{
			cout << e.f << '\n';
		}
	}

	_getch();
	return 0;
}