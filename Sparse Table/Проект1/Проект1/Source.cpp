#include<iostream>
//#include<conio.h>
#include<vector>
#include<algorithm>

using namespace std;

//Нумерация с 1
class SparseTable
{
public:
	SparseTable(vector<int>* values)
	{
		this->values = values;
		int count = values->size();
		table.resize(count + 1);
		logs2.resize(count + 1);
		pows2[0] = 1;
		for (int i = 1; i < 31; i++)
			pows2[i] = pows2[i - 1] * 2;

		for (int i = 1; i <= count; i++)
		{
			int j = 0;
			while (pows2[j] <= count)
			{
				j++;
			}
			table[i].resize(j, INT32_MIN);
			
			j = 0;
			while (pows2[j] <= i)
			{
				j++;
			}
			logs2[i] = j - 1;
		}
	}

	int GetMin(int left, int right)
	{
		return max(Get(left, logs2[right - left + 1]),
				   Get(right - pows2[logs2[right - left + 1]] + 1, logs2[right - left + 1]));
	}

private:
	vector< vector<int> > table;
	vector<int> logs2;
	vector<int>* values = NULL;
	int pows2[31];

	int Get(int i, int j)
	{
		if (table[i][j] != INT32_MIN)
			return table[i][j];
		if (j == 0)
			return (*values)[i];
		table[i][j] = max(Get(i, j - 1), Get(i + pows2[j - 1], j - 1));
		return table[i][j];
	}
};

int main()
{
	ios_base::sync_with_stdio(0), cin.tie(0);
	int n;
	cin >> n;

	vector<int> a;
	a.resize(n + 1);
	for (int i = 1; i <= n; i++)
	{
		cin >> a[i];
	}

	SparseTable* st = new SparseTable(&a);
	
	int k;
	cin >> k;
	for (int i = 0; i < k; i++)
	{
		int l, r;
		cin >> l >> r;
		cout << st->GetMin(l, r) << ' ';
	}

	//_getch();
	return 0;
}