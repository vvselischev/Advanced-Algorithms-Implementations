#include<iostream>
#include<conio.h>
#include<vector>
#include<algorithm>
#include<map>

using namespace std;

const long long INF = INT64_MAX;

struct edge
{
	int to;
	int weight;
};

vector< vector<edge> > graph;
vector<long long> distances;
vector<int> parents;

void Dijkstra(int start, int n)
{
	distances.resize(n + 1, INF);
	parents.resize(n + 1);
	vector<bool> visited;
	visited.resize(n + 1, false);

	multimap<long long, int> closestVerts;

	distances[start] = 0;
	parents[start] = start;
	int currentVert = start;
	closestVerts.emplace(0, start);

	for (int k = 0; k < n; k++)
	{
		visited[currentVert] = true;
		for (int i = 0; i < graph[currentVert].size(); i++)
		{
			edge currentEdge = graph[currentVert][i];
			if (distances[currentEdge.to] > distances[currentVert] + currentEdge.weight)
			{
				distances[currentEdge.to] = distances[currentVert] + currentEdge.weight;
				closestVerts.emplace(distances[currentEdge.to], currentEdge.to);
				parents[currentEdge.to] = currentVert;
			}
		}

		multimap<long long, int>::iterator it = closestVerts.begin();
		while (!closestVerts.empty())
		{
			if (visited[it->second])
			{
				closestVerts.erase(it);
				it = closestVerts.begin();
			}
			else
			{
				currentVert = it->second;
				break;
			}
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(0), cin.tie(0);

	int n, start, finish;
	cin >> n >> start >> finish;

	//—читывание графа
	graph.resize(n + 1);
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			int a;
			cin >> a;
			if (a > 0)
			{
				graph[i].push_back({ j, a });
			}
		}
	}

	Dijkstra(start, n);
	if (distances[finish] == INF)
	{
		cout << -1;
	}
	else
	{
		int currentVert = finish;
		vector<int> path;
		path.push_back(currentVert);
		while (parents[currentVert] != currentVert)
		{
			currentVert = parents[currentVert];
			path.push_back(currentVert);
		}
		reverse(path.begin(), path.end());

		for (int i = 0; i < path.size(); i++)
		{
			cout << path[i] << ' ';
		}
	}

	_getch();
	return 0;
}