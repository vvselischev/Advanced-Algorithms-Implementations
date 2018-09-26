#include<iostream>
#include<conio.h>
#include<algorithm>
#include<vector>

using namespace std;

//В вершине наибольший приоритет, нумерация с 0
class Treap
{
public:
	long long value;
	int key;
	Treap* left;
	Treap* right;
	Treap* parent;

	Treap(int key, int priority, int value, Treap* left = NULL, Treap* right = NULL, Treap* parent = NULL)
	{
		this->key = key;
		this->priority = priority;
		this->value = value;
		this->left = left;
		this->right = right;
		this->parent = parent;
		size = -1;
		sum = value;
		myMax = value;
		delta = 0;
		Recalc();
	}

	static int GetSize(Treap* treap)
	{
		if (treap == NULL)
			return 0;
		return treap->size;
	}

	static long long GetMax(Treap* treap, int left, int right)
	{
		pair<Treap*, Treap*> splitted1 = Split(treap, left - 1);
		pair<Treap*, Treap*> splitted2 = Split(splitted1.second, right);
		long long res = GetMax(splitted2.first);
		treap = Merge(splitted1.first, Merge(splitted2.first, splitted2.second));
		return res;
	}

	static long long GetMax(Treap* treap)
	{
		if (treap == NULL)
			return INT64_MIN;
		return treap->myMax;
	}

	static long long GetSum(Treap* treap, int left, int right)
	{
		pair<Treap*, Treap*> splitted1 = Split(treap, left - 1);
		pair<Treap*, Treap*> splitted2 = Split(splitted1.second, right);
		long long res = GetSum(splitted2.first);
		treap = Merge(splitted1.first, Merge(splitted2.first, splitted2.second));
		return res;
	}

	static long long GetSum(Treap* treap)
	{
		if (treap == NULL)
			return 0;
		return treap->sum + treap->size * treap->delta;
	}

	//Пока что возвращает 0, если такого ключа нет. Можно проверять подотрезок на размер.
	static long long GetValue(Treap* treap, int key)
	{
		return GetSum(treap, key, key);
	}

	static Treap* Add(Treap* treap, int key, int priority, long long value)
	{
		Treap* tmp = new Treap(key, priority, value, NULL, NULL);
		pair<Treap*, Treap*> splitted = Split(treap, key);
		treap = Merge(Merge(splitted.first, tmp), splitted.second);
		return treap;
	}

	static Treap* Remove(Treap* treap, int key)
	{
		pair<Treap*, Treap*> splitted = Split(treap, key - 1);
		Treap* first = splitted.first;
		splitted = splitted.second->Split(treap, key);
		return Merge(first, splitted.second);
	}

	static Treap* Update(Treap* treap, int left, int right, long long delta)
	{
		pair<Treap*, Treap*> splitted1 = Split(treap, left - 1);
		pair<Treap*, Treap*> splitted2 = Split(splitted1.second, right);
		splitted2.first->delta += delta;
		return Merge(splitted1.first, Merge(splitted2.first, splitted2.second));
	}

	static Treap* Build(vector<int> x, vector<int> y, vector<int> values, int count)
	{
		Treap* tree = new Treap(x[0], y[0], values[0]);
		Treap* last = tree;

		for (int i = 1; i < count; ++i)
		{
			if (last->priority > y[i])
			{
				last->right = new Treap(x[i], y[i], values[i], NULL, NULL, last);
				last = last->right;
			}
			else
			{
				Treap* cur = last;
				while ((cur->parent != NULL) && (cur->priority <= y[i]))
					cur = cur->parent;
				if (cur->priority <= y[i])
					last = new Treap(x[i], y[i], values[i], cur);
				else
				{
					last = new Treap(x[i], y[i], values[i], cur->right, NULL, cur);
					cur->right = last;
				}
			}
		}

		while (last->parent != NULL)
			last = last->parent;
		return last;
	}

private:
	int priority;
	long long size;
	long long sum;
	long long myMax;
	long long delta;

	void Push()
	{
		if (left != NULL)
		{
			left->delta += delta;
		}
		if (right != NULL)
		{
			right->delta += delta;
		}
		value += delta;
		delta = 0;
	}

	void Recalc()
	{
		Push();
		size = GetSize(left) + GetSize(right) + 1;
		sum = GetSum(left) + GetSum(right) + value;
		myMax = max(max(value, GetMax(left)), GetMax(right));
	}

	static Treap* Merge(Treap* first, Treap* second)
	{
		if (first == NULL)
			return second;
		if (second == NULL)
			return first;

		Treap* merged = NULL;
		
		first->Recalc();
		second->Recalc();  
		if (first->priority > second->priority)
		{
			Treap* tmp = Merge(first->right, second);
			first->right = tmp;
			merged = first;
		}
		else
		{
			Treap* tmp = Merge(first, second->left);
			second->left = tmp;
			merged = second;
		}
		
		merged->Recalc();
		return merged;
	}

	static pair<Treap*, Treap*> Split(Treap* treap, int key)
	{
		Treap* first = NULL;
		Treap* second = NULL;
		if (treap == NULL)
		{
			return make_pair(first, second);
		}
		else
		{
			treap->Push();
			if (key >= treap->key)
			{
				pair<Treap*, Treap*> splitted = Split(treap->right, key);
				treap->right = splitted.first;
				second = splitted.second;
				first = treap;
				first->Recalc();
			}
			else
			{
				pair<Treap*, Treap*> splitted = Split(treap->left, key);
				first = splitted.first;
				treap->left = splitted.second;
				second = treap;
				second->Recalc();
			}
		}
		return make_pair(first, second);
	}
};

int main()
{
	ios_base::sync_with_stdio(0), cin.tie(0);
	int n;
	cin >> n;

	int value;
	cin >> value;

	Treap* treap = new Treap(0, rand(), value, NULL, NULL);
	for (int i = 1; i < n; i++)
	{
		cin >> value;
		treap = Treap::Add(treap, i, rand(), value);
	}

	int k;
	cin >> k;
	for (int i = 0; i < k; i++)
	{
		char c;
		cin >> c;
		if (c == 'u')
		{
			int id, delta;
			cin >> id >> delta;
			treap = Treap::Update(treap, id - 1, id - 1, -Treap::GetValue(treap, id - 1) + delta);
		}
		else
		{
			int l, r;
			cin >> l >> r;
			cout << Treap::GetSum(treap, l - 1, r - 1) << ' ';
			//cout << Treap::GetMax(treap, l - 1, r - 1) << ' ';
		}
	}

	_getch();
	return 0;
}