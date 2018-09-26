#include<iostream>
#include<conio.h>
#include<vector>
#include<algorithm>

using namespace std;

long long table[301][301];
long long rowDelta[301];
long long columnDelta[301];
long long pairB[301];
long long pairA[301];
long long prevB[301];
pair<long long, long long> w[301];
long long n;
bool visited[301];
bool markedA[301];
bool markedB[301];

int main()
{
	long long n;
	cin >> n;
	for (long long i = 0; i < n; i++)
	{
		for (long long j = 0; j < n; j++)
		{
			cin >> table[i][j];
		}
	}

	fill(pairB, pairB + n, -1);

	for (long long i = 0; i < n; i++)
	{
		fill(markedA, markedA + n, false);
		fill(markedB, markedB + n, false);
		fill(prevB, prevB + n, -1);
		markedA[i] = true;
		long long column = 0;
		for (long long j = 0; j < n; j++)
		{
			w[j] = { table[i][j] + rowDelta[i] + columnDelta[j], i };
		}
		while (true)
		{
			long long x = INT64_MAX / 2;
			long long row = 0;

			for (long long j = 0; j < n; j++)
			{
				if (!markedB[j] && w[j].first < x)
				{
					x = w[j].first;
					column = j;
					row = w[j].second;
				}
			}

			for (long long k = 0; k < n; k++)
			{
				if (!markedA[k])
				{
					rowDelta[k] += x;
				}
				if (!markedB[k])
				{
					columnDelta[k] -= x;
					w[k].first -= x;
				}
			}
			markedB[column] = true;
			prevB[column] = row;
			if (pairB[column] == -1)
			{
				break;
			}
			markedA[pairB[column]] = true;
			for (long long j = 0; j < n; j++)
			{
				w[j] = min(w[j], { table[pairB[column]][j] + rowDelta[pairB[column]] + columnDelta[j], pairB[column] });
			}
		}

		long long currentColumn = column;
		while (true)
		{
			pairB[currentColumn] = prevB[currentColumn];
			long long tmp = pairA[prevB[currentColumn]];
			pairA[prevB[currentColumn]] = currentColumn;
			if (prevB[currentColumn] == i)
			{
				break;
			}
			currentColumn = tmp;
		}
	}

	long long total = 0;
	for (long long i = 0; i < n; i++)
	{
		total += table[i][pairA[i]];
	}

	cout << total << '\n';
	for (long long i = 0; i < n; i++)
	{
		cout << i + 1 << ' ' << pairA[i] + 1 << '\n';
	}

	_getch();
	return 0;
}