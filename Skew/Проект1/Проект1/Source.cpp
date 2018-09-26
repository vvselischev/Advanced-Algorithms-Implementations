#include<iostream>
//#include<conio.h>

using namespace std;

template<class T>
class Node
{
public:
	Node* left = NULL;
	Node* right = NULL;
	T value;

	Node(T value)
	{
		this->value = value;
	}
};

template<class T>
class Skew
{
private:
	Node<T>* node = NULL;

public:
	void Add(T value)
	{
		Node<T>* tmp = new Node<T>(value);
		node = Merge(node, tmp);
	}

	T GetMin()
	{
		return node->value;
	}

	T DeleteMin()
	{
		T res = node->value;
		node = Merge(node->left, node->right);
		return res;
	}

	Node<T>* Merge(Node<T>* first, Node<T>* second)
	{
		if (first == NULL)
			return second;
		if (second == NULL)
			return first;
		if (first->value > second->value)
		{
			swap(first, second);
		}
		Node<T>* tmp = Merge(first->right, second);
		first->right = tmp;
		swap(first->right, first->left);
		return first;
	}
};


int main()
{
	int n;
	cin >> n;
	int a;
	Skew<int> s;
	for (int i = 0; i < n; i++)
	{
		cin >> a;
		s.Add(a);
	}

	for (int i = 0; i < n; i++)
	{
		cout << s.DeleteMin() << ' ';
	}

	//_getch();
	return 0;
}