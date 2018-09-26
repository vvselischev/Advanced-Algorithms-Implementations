#include<iostream>
#include<conio.h>

using namespace std;

//Дерево Фенвика для суммы с чтением элемента за О(1), но О(n) доп. памяти, нумерация с 0
//Чтобы сделать чтение за О(logn), тупо sum(i, i)
template<class T>
class Fenwick 
{
public:
	Fenwick(T* data, int count)
	{
		this->count = count;
		this->data = new T[count];
		partSum = new T[count];

		for (int i = 0; i < count; i++)
		{
			partSum[i] = 0;
		}

		for (int i = 0; i < count; i++)
		{
			change(i, data[i]);
		}
	}

	T get(int i)
	{
		return data[i];
	}

	int getCount()
	{
		return count;
	}
	
	void add(int i, T delta)
	{
		data[i] += delta;
		while (i < count)
		{
			partSum[i] += delta;
			i = i | (i + 1);
		}	
	}

	void change(int i, T value)
	{
		add(i, -data[i]);
		add(i, value);
	}

	T sum(int l, int r)
	{
		return sum(r) - sum(l - 1);
	}

private:
	int count;
	T* partSum;
	T* data;

	T sum(int r)
	{
		T result = 0;
		while (r >= 0)
		{
			result = result + partSum[r];
			r = (r & (r + 1)) - 1;
		}
		return result;
	}
};


int main()
{
	int n;
	cin >> n;
	long long* a = new long long[n];

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	Fenwick<long long> fenwick(a, n);
	
	int m;
	cin >> m;
	for (int q = 0; q < m; q++)
	{
		char c;
		cin >> c;
		if (c == 's')
		{
			int l, r;
			cin >> l >> r;
			cout << fenwick.sum(l - 1, r - 1) << ' ';
		}
		else
		{
			int i;
			long long value;
			cin >> i >> value;
			fenwick.change(i - 1, value);
		}
	}

	_getch();
	return 0;
}