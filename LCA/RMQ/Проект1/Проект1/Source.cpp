#include<iostream>
#include<conio.h>
#include<algorithm>
#include<vector>

using namespace std;

//В вершине наибольший приоритет, нумерация с 1
class ImplicitTreap
{
public:
	long long value;
	ImplicitTreap* left;
	ImplicitTreap* right;

	ImplicitTreap(int priority, int value, ImplicitTreap* left = NULL, ImplicitTreap* right = NULL)
	{
		this->priority = priority;
		this->value = value;
		this->left = left;
		this->right = right;
		size = -1;
		sum = value;
		myMin = value;
		reverse = false;
		delta = 0;
		Recalc();
	}

	static int GetSize(ImplicitTreap* treap)
	{
		if (treap == NULL)
			return 0;
		return treap->size;
	}

	static long long GetMin(ImplicitTreap* treap, int left, int right)
	{
		pair<ImplicitTreap*, ImplicitTreap*> splitted1 = Split(treap, left - 1);
		pair<ImplicitTreap*, ImplicitTreap*> splitted2 = Split(splitted1.second, right - left + 1);
		long long res = GetMin(splitted2.first);
		treap = Merge(splitted1.first, Merge(splitted2.first, splitted2.second));
		return res;
	}

	static long long GetMin(ImplicitTreap* treap)
	{
		if (treap == NULL)
			return INT64_MAX;
		return treap->myMin;
	}

	static long long GetSum(ImplicitTreap* treap, int left, int right)
	{
		pair<ImplicitTreap*, ImplicitTreap*> splitted1 = Split(treap, left - 1);
		pair<ImplicitTreap*, ImplicitTreap*> splitted2 = Split(splitted1.second, right - left + 1);
		long long res = GetSum(splitted2.first);
		treap = Merge(splitted1.first, Merge(splitted2.first, splitted2.second));
		return res;
	}

	static long long GetSum(ImplicitTreap* treap)
	{
		if (treap == NULL)
			return 0;
		return treap->sum + treap->size * treap->delta;
	}

	//Пока что возвращает 0, если такого ключа нет. Можно проверять подотрезок на размер.
	static long long GetValue(ImplicitTreap* treap, int i)
	{
		return GetSum(treap, i, i);
	}

	static ImplicitTreap* Add(ImplicitTreap* treap, int i, int priority, long long value)
	{
		ImplicitTreap* tmp = new ImplicitTreap(priority, value);
		pair<ImplicitTreap*, ImplicitTreap*> splitted = Split(treap, i - 1);
		treap = Merge(Merge(splitted.first, tmp), splitted.second);
		return treap;
	}

	static ImplicitTreap* Remove(ImplicitTreap* treap, int i)
	{
		pair<ImplicitTreap*, ImplicitTreap*> splitted = Split(treap, i - 1);
		ImplicitTreap* first = splitted.first;
		splitted = splitted.second->Split(treap, 1);
		return Merge(first, splitted.second);
	}

	static ImplicitTreap* Update(ImplicitTreap* treap, int left, int right, long long delta)
	{
		pair<ImplicitTreap*, ImplicitTreap*> splitted1 = Split(treap, left - 1);
		pair<ImplicitTreap*, ImplicitTreap*> splitted2 = Split(splitted1.second, right - left + 1);
		splitted2.first->delta += delta;
		return Merge(splitted1.first, Merge(splitted2.first, splitted2.second));
	}

	static ImplicitTreap* Reverse(ImplicitTreap* treap, int left, int right)
	{
		pair<ImplicitTreap*, ImplicitTreap*> splitted1 = Split(treap, left - 1);
		pair<ImplicitTreap*, ImplicitTreap*> splitted2 = Split(splitted1.second, right - left + 1);
		splitted2.first->reverse ^= true;
		return Merge(splitted1.first, Merge(splitted2.first, splitted2.second));
	}

private:
	int priority;
	long long size;
	long long sum;
	long long myMin;
	long long delta;
	bool reverse;

	void Push()
	{
		if (left != NULL)
		{
			left->delta += delta;
			left->reverse ^= reverse;
		}
		if (right != NULL)
		{
			right->delta += delta;
			right->reverse ^= reverse;
		}
		value += delta;
		delta = 0;
		if (reverse)
		{
			swap(left, right);
			reverse = false;
		}
	}

	void Recalc()
	{
		Push();
		size = GetSize(left) + GetSize(right) + 1;
		sum = GetSum(left) + GetSum(right) + value;
		myMin = min(min(value, GetMin(left)), GetMin(right));
	}

	static ImplicitTreap* Merge(ImplicitTreap* first, ImplicitTreap* second)
	{
		if (first == NULL)
			return second;
		if (second == NULL)
			return first;

		first->Recalc();
		second->Recalc();
		ImplicitTreap* merged = NULL;
		if (first->priority > second->priority)
		{
			ImplicitTreap* tmp = Merge(first->right, second);
			first->right = tmp;
			merged = first;
		}
		else
		{
			ImplicitTreap* tmp = Merge(first, second->left);
			second->left = tmp;
			merged = second;
		}

		merged->Recalc();
		return merged;
	}

	static pair<ImplicitTreap*, ImplicitTreap*> Split(ImplicitTreap* treap, int key)
	{
		ImplicitTreap* first = NULL;
		ImplicitTreap* second = NULL;
		if (treap == NULL)
		{
			return make_pair(first, second);
		}

		treap->Recalc();
		if (key >= GetSize(treap->left) + 1)
		{
			pair<ImplicitTreap*, ImplicitTreap*> splitted = Split(treap->right, key - GetSize(treap->left) - 1);
			treap->right = splitted.first;
			second = splitted.second;
			first = treap;
			first->Recalc();
		}
		else
		{
			pair<ImplicitTreap*, ImplicitTreap*> splitted = Split(treap->left, key);
			first = splitted.first;
			treap->left = splitted.second;
			second = treap;
			second->Recalc();
		}
		return make_pair(first, second);
	}
};

vector<long long> vertsOfEnterTime; //в какую вершину зашли в i-й момент времени
vector<long long> enterTime; //время входа в i-ю вершину
ImplicitTreap* treap;
vector< vector<long long> > graph;

long long timer;
void DFS(int vert, int from)
{
	vertsOfEnterTime.push_back(vert);
	enterTime[vert] = timer;
	if (vert != 0)
	{	
		treap = ImplicitTreap::Add(treap, timer, rand(), enterTime[vert]);
	}
	timer++;
	for (int i = 0; i < graph[vert].size(); i++)
	{
		int to = graph[vert][i];
		if (to != from)
		{
			DFS(to, vert);
			treap = ImplicitTreap::Add(treap, timer, rand(), enterTime[vert]);
			vertsOfEnterTime.push_back(vert);
			timer++;
		}	
	}
}

long long GetLCA(long long first, long long second)
{
	long long firstTime = enterTime[first];
	long long secondTime = enterTime[second];
	if (firstTime > secondTime)
		swap(firstTime, secondTime);
	long long lcaEnterTime = ImplicitTreap::GetMin(treap, firstTime, secondTime);
	return vertsOfEnterTime[lcaEnterTime];
}

int main()
{
	ios_base::sync_with_stdio(0), cin.tie(0);
	int n, m;
	cin >> n >> m;

	//Чтение графа
	graph.resize(n);
	for (int i = 1; i <= n - 1; i++)
	{
		int a;
		cin >> a;
		graph[i].push_back(a);
		graph[a].push_back(i);
	}

	//Предподсчёт
	enterTime.resize(n); //вершины нумеруются с 0
	timer = 1;
	vertsOfEnterTime.push_back(-1); //т.к. время с 1
	treap = new ImplicitTreap(rand(), 1);
	DFS(0, -1);

	for (int i = 0; i < m; i++)
	{
		int a, b;
		cin >> a >> b;
		cout << GetLCA(a, b) << endl;
	}

	_getch();
	return 0;
}