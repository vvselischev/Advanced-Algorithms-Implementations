#include<iostream>
#include<conio.h>

using namespace std;

const int MAXN = 1e5;
long long tree[4 * MAXN + 10];

long long makeParent(long long a, long long b)
{
	return a + b;
}

void Build(int* a, int current, int left, int right)
{
	if (left == right)
	{
		tree[current] = a[left];
	}
	else
	{
		int middle = left + (right - left) / 2;
		Build(a, 2 * current, left, middle);
		Build(a, 2 * current + 1, middle + 1, right);
		tree[current] = makeParent(tree[2 * current], tree[2 * current + 1]);
	}
}

long long Find(int current, int count, int left, int right)
{
	if (right - left + 1 == count)
	{
		return tree[current];
	}

	int countRight = count / 2;
	int countLeft = count - countRight;

	if (left > countLeft)
	{
		return Find(2 * current + 1, countRight, left - countLeft, right - countLeft);
	}
	if (right <= countLeft)
	{
		return Find(2 * current, countLeft, left, right);
	}
	return makeParent(Find(2 * current, countLeft, left, countLeft),
		Find(2 * current + 1, countRight, 1, right - countLeft));

}

void Update(int current, int count, int left, int right, int newValue)
{
	if (right - left + 1 == count)
	{
		tree[current] = newValue;
		return;
	}

	int countRight = count / 2;
	int countLeft = count - countRight;

	if (left > countLeft)
	{
		Update(2 * current + 1, countRight, left - countLeft, right - countLeft, newValue);
		tree[current] = makeParent(tree[2 * current], tree[2 * current + 1]);
		return;
	}
	if (right <= countLeft)
	{
		Update(2 * current, countLeft, left, right, newValue);
		tree[current] = makeParent(tree[2 * current], tree[2 * current + 1]);
		return;
	}
	Update(2 * current, countLeft, left, countLeft, newValue);
	Update(2 * current + 1, countRight, 1, right - countLeft, newValue);
	tree[current] = makeParent(tree[2 * current], tree[2 * current + 1]);
}


int a[MAXN + 1];
int main()
{
	int n, q;
	cin >> n >> q;

	Build(a, 1, 1, n);

	for (int i = 0; i < q; i++)
	{
		char c;
		int x, y;
		cin >> c >> x >> y;
		if (c == 'A')
		{
			Update(1, n, x, x, y);
		}
		else
		{
			cout << Find(1, n, x, y) << '\n';
		}
	}

	_getch();
	return 0;
}
