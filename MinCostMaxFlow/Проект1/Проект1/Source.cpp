#include<iostream>
#include<conio.h>
#include<vector>
#include<algorithm>

using namespace std;

struct Edge
{
	long long a, b, f, c, w;
	long long next;
};

vector<Edge> edges;
long long d[501];
int head[501];
int last[501];
long long n, m, s, t;


bool fordBellman()
{
	fill(d, d + n, INT64_MAX / 100);
	d[s] = 0;
	for (long long q = 0; q <= n; q++)
	{
		bool changed = false;
		for (Edge e : edges)
		{
			if (e.c > e.f && d[e.a] < INT64_MAX / 100 && d[e.b] > d[e.a] + e.w)
			{
				d[e.b] = d[e.a] + e.w;
				changed = true;
			}
		}
		if (!changed)
		{
			break;
		}
	}
	return d[t] < INT64_MAX / 100;
}

long long ans = 0;
bool used[501];

long long dfs(int v, long long curDelta)
{
	if (used[v]) return 0;
	used[v] = true;
	if (v == t || curDelta == 0)
	{
		return curDelta;
	}
	int i = head[v];
	while (i != -1)
	{
		Edge e = edges[i];
		if (d[e.a] + e.w == d[e.b])
		{
			if (!used[e.b] && e.f < e.c)
			{
				long long newDelta = min(curDelta, e.c - e.f);
				if (e.b != t)
				{
					newDelta = dfs(e.b, newDelta);
				}
				if (e.b == t || newDelta > 0)
				{
					long long delta = min(curDelta, newDelta);
					edges[i].f += delta;
					edges[i ^ 1].f -= delta;
					ans += delta * e.w;
					return delta;
				}
			}
		}
		i = edges[i].next;
	}
	return 0;
}

int main()
{
	cin >> n >> m;
	s = 0;
	t = n - 1;
	fill(head, head + n, -1);
	for (long long i = 0; i < m; i++)
	{
		long long a, b, c, w;
		cin >> a >> b >> c >> w;
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
		edges.push_back({ a, b, 0, c, w, -1 });

		if (head[b] == -1)
		{
			head[b] = edges.size();
		}
		else
		{
			edges[last[b]].next = edges.size();
		}
		last[b] = edges.size();
		edges.push_back({ b, a, 0, 0, -w, -1 });
	}

	while (fordBellman())
	{
		while (true)
		{
			fill(used, used + n, false);
			if (dfs(s, INT64_MAX / 2) <= 0)
			{
				break;
			}
		}
	}

	cout << ans;

	_getch();
	return 0;
}