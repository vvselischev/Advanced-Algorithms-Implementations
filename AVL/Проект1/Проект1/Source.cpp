#include<iostream>
#include<conio.h>
#include<algorithm>
#include "optimization.h"

using namespace std;

class AVLTree
{
public:
	long long value;

	AVLTree(long long value, AVLTree* left = NULL, AVLTree* right = NULL, AVLTree* parent = NULL)
	{
		this->value = value;
		this->left = left;
		this->right = right;
		this->parent = parent;
		this->size = GetSize(left) + GetSize(right) + 1;
		this->height = max(GetHeight(left), GetHeight(right)) + 1;
		this->sum = GetSum(left) + GetSum(right) + value;
		this->myMin = min(value, min(GetValue(left), GetValue(right)));
		this->myMax = max(value, max(GetValue(left), GetValue(right)));
	}

	static long long GetHeight(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return 0;
		}
		return tree->height;
	}

	static long long GetSize(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return 0;
		}
		return tree->size;
	}

	static long long GetValue(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return 0;
		}
		return tree->value;
	}

	static long long GetSum(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return 0;
		}
		return tree->sum;
	}

	static long long GetMin(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return INT32_MAX;
		}
		return tree->myMin;
	}

	static long long GetMax(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return INT32_MIN;
		}
		return tree->myMax;
	}

	//Need to test!!!
	static long long GetSumSegment(AVLTree* tree, long long l, long long r)
	{
		if (l > r)
		{
			return 0;
		}
		if (l == r)
		{
			return GetValue(tree);
		}
		if (r <= GetSize(GetLeft(tree)))
		{
			return GetSumSegment(GetLeft(tree), l, r);
		}
		if (l > GetSize(GetLeft(tree)))
		{
			return GetSumSegment(GetRight(tree), l - GetSize(GetLeft(tree)) - 1,
				r - GetSize(GetLeft(tree)) - 1);
		}
		return GetSumSegment(GetLeft(tree), l, GetSize(GetLeft(tree)) - 1) + GetValue(tree) +
			GetSumSegment(GetRight(tree), 1, r - GetSize(GetLeft(tree)) - 1);
	}

	static long long GetSumKeyInterval(AVLTree* tree, long long l, long long r)
	{
		if (l > r || tree == NULL)
		{
			return 0;
		}

		if (r < GetValue(tree))
		{
			return GetSumKeyInterval(GetLeft(tree), l, r);
		}
		if (l > GetValue(tree))
		{
			return GetSumKeyInterval(GetRight(tree), l, r);
		}
		if (GetValue(tree) >= l && GetValue(tree) <= r)
		{
			return GetSumKeyInterval(GetLeft(tree), l, r) + GetValue(tree) +
				GetSumKeyInterval(GetRight(tree), l, r);
		}
		return 0;
	}

	static pair<AVLTree*, AVLTree*> Split(AVLTree* tree, long long value)
	{
		if (tree == NULL)
		{
			return{ NULL, NULL };
		}

		AVLTree* first = NULL;
		AVLTree* second = NULL;
		if (value > GetValue(tree))
		{
			if (GetRight(tree) == NULL)
			{
				return make_pair(tree, second);
			}
			else
			{
				pair<AVLTree*, AVLTree*> splittedSon = Split(GetRight(tree), value);
				tree = SetRight(tree, splittedSon.first);
				first = tree;
				second = splittedSon.second;
			}
		}
		else
		{
			if (GetLeft(tree) == NULL)
			{
				return make_pair(first, tree);
			}
			else
			{
				pair<AVLTree*, AVLTree*> splittedSon = Split(GetLeft(tree), value);
				tree = SetLeft(tree, splittedSon.second);
				second = tree;
				first = splittedSon.first;
			}
		}
		//delete tree ?? (before SetLeft(tree, NULL), SetRight...)
		return make_pair(Balance(first), Balance(second));
	}

	static AVLTree* Merge(AVLTree* leftTree, AVLTree* rightTree)
	{
		if (leftTree == NULL)
		{
			return rightTree;
		}
		if (rightTree == NULL)
		{
			return leftTree;
		}

		leftTree = Recalc(leftTree);
		rightTree = Recalc(rightTree);
		AVLTree* merged = NULL;
		if (GetHeight(leftTree) > GetHeight(rightTree))
		{
			leftTree = SetRight(leftTree, Merge(GetRight(leftTree), rightTree));
			merged = leftTree;
		}
		else
		{
			rightTree = SetLeft(rightTree, Merge(leftTree, GetLeft(rightTree)));
			merged = rightTree;
		}
		return Balance(merged);
	}

	static AVLTree* Next(AVLTree* tree)
	{
		if (GetRight(tree) != NULL)
		{
			return GetMinNode(GetRight(tree));
		}
		while (GetRight(tree->parent) == tree)
		{
			tree = tree->parent;
		}
		return tree->parent;
	}

	static long long GetKthMin(AVLTree* tree, int k)
	{
		if (k <= GetSize(GetLeft(tree)))
		{
			return GetKthMin(GetLeft(tree), k);
		}
		if (k == GetSize(GetLeft(tree)) + 1)
		{
			return GetValue(tree);
		}
		return GetKthMin(GetRight(tree), k - GetSize(GetLeft(tree)) - 1);
	}

	static AVLTree* GetMinNode(AVLTree* tree)
	{
		if (GetLeft(tree) == NULL)
		{
			return tree;
		}
		return GetMinNode(GetLeft(tree));
	}

	static bool Contains(AVLTree* tree, long long value)
	{
		if (tree == NULL)
		{
			return false;
		}
		return Contains(GetLeft(tree), value) || value == GetValue(tree) ||
			Contains(GetRight(tree), value);
	}

	static AVLTree* Add(AVLTree* tree, long long value)
	{
		if (tree == NULL)
		{
			return new AVLTree(value);
		}
		if (value < tree->value)
		{
			tree = SetLeft(tree, Add(GetLeft(tree), value));
		}
		else
		{
			tree = SetRight(tree, Add(GetRight(tree), value));
		}
		return Balance(tree);
	}

	static AVLTree* AddUnique(AVLTree* tree, long long value)
	{
		if (Contains(tree, value))
		{
			return tree;
		}
		return Add(tree, value);
	}

	static AVLTree* RemoveMin(AVLTree* tree)
	{
		if (GetLeft(tree) == NULL)
		{
			return GetRight(tree);
		}
		tree = SetLeft(tree, RemoveMin(GetLeft(tree)));
		return Balance(tree);
	}

	static AVLTree* Remove(AVLTree* tree, long long value)
	{
		if (tree == NULL)
		{
			return NULL;
		}
		if (value < GetValue(tree))
		{
			tree = SetLeft(tree, Remove(GetLeft(tree), value));
			return Balance(tree);
		}
		else
		{
			if (value > GetValue(tree))
			{
				tree = SetRight(tree, Remove(GetRight(tree), value));
				return Balance(tree);
			}
			else
			{
				AVLTree* oldLeft = GetLeft(tree);
				AVLTree* oldRight = GetRight(tree);
				delete tree;
				if (oldRight == NULL)
				{
					return Balance(oldLeft);
				}
				if (oldLeft == NULL)
				{
					return Balance(oldRight);
				}
				AVLTree* minNode = GetMinNode(oldRight);
				minNode = SetRight(minNode, RemoveMin(oldRight));
				minNode = SetLeft(minNode, oldLeft);
				return Balance(minNode);
			}
		}
	}

	static AVLTree* GetLeft(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return NULL;
		}
		return tree->left;
	}

	static AVLTree* GetRight(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return NULL;
		}
		return tree->right;
	}

	static AVLTree* SetLeft(AVLTree* sourceTree, AVLTree* destTree)
	{
		if (destTree != NULL)
		{
			destTree->parent = sourceTree;
		}
		if (sourceTree == NULL)
		{
			return destTree;
		}
		sourceTree->left = destTree;
		return sourceTree;
	}

	static AVLTree* SetRight(AVLTree* sourceTree, AVLTree* destTree)
	{
		if (destTree != NULL)
		{
			destTree->parent = sourceTree;
		}
		if (sourceTree == NULL)
		{
			return destTree;
		}
		sourceTree->right = destTree;
		return sourceTree;
	}

private:
	long long size;
	long long myMin;
	long long myMax;
	long long height;
	long long sum;
	AVLTree* parent;
	AVLTree* left;
	AVLTree* right;

	static AVLTree* Recalc(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return NULL;
		}
		tree->height = max(GetHeight(GetLeft(tree)), GetHeight(GetRight(tree))) + 1;
		tree->size = GetSize(GetLeft(tree)) + GetSize(GetRight(tree)) + 1;
		tree->sum = GetSum(GetLeft(tree)) + GetSum(GetRight(tree)) + GetValue(tree);
		tree->myMin = min(GetValue(tree), min(GetMin(GetLeft(tree)), GetMin(GetRight(tree))));
		tree->myMax = max(GetValue(tree), max(GetMax(GetLeft(tree)), GetMax(GetRight(tree))));
		return tree;
	}

	static long long GetBalance(AVLTree* tree)
	{
		return GetHeight(GetRight(tree)) - GetHeight(GetLeft(tree));
	}

	static AVLTree* SmallRotateRight(AVLTree* tree)
	{
		if (GetLeft(tree) == NULL)
		{
			return NULL;
		}
		AVLTree* leftSon = GetLeft(tree);
		tree = SetLeft(tree, GetRight(leftSon));
		leftSon = SetRight(leftSon, tree);
		tree = Recalc(tree);
		leftSon = Recalc(leftSon);
		return leftSon;
	}

	static AVLTree* SmallRotateLeft(AVLTree* tree)
	{
		if (GetRight(tree) == NULL)
		{
			return NULL;
		}
		AVLTree* rightSon = GetRight(tree);
		tree = SetRight(tree, GetLeft(rightSon));
		rightSon = SetLeft(rightSon, tree);
		tree = Recalc(tree);
		rightSon = Recalc(rightSon);
		return rightSon;
	}

	static AVLTree* BigRotateLeft(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return NULL;
		}
		if (GetBalance(GetRight(tree)) >= 0)
		{
			return SmallRotateLeft(tree);
		}
		tree = SetRight(tree, SmallRotateRight(GetRight(tree)));
		tree = Recalc(tree);
		return SmallRotateLeft(tree);
	}

	static AVLTree* BigRotateRight(AVLTree* tree)
	{
		if (tree == NULL)
		{
			return NULL;
		}
		if (GetBalance(GetLeft(tree)) < 0)
		{
			return SmallRotateRight(tree);
		}
		tree = SetLeft(tree, SmallRotateLeft(GetLeft(tree)));
		tree = Recalc(tree);
		return SmallRotateRight(tree);
	}

	static AVLTree* Balance(AVLTree* tree)
	{
		tree = Recalc(tree);
		if (GetBalance(tree) == 2)
		{
			return BigRotateLeft(tree);
		}
		if (GetBalance(tree) == -2)
		{
			return BigRotateRight(tree);
		}
		return tree;
	}
};

int main()
{
	int n;
	//cin >> n;
	n = readInt();

	int c, k;
	//cin >> c >> k;
	c = readInt(), k = readInt();

	AVLTree* tree = new AVLTree(-k);

	for (int q = 1; q < n; q++)
	{
		//cin >> c >> k;
		c = readInt(), k = readInt();
		if (c == 1)
		{
			tree = AVLTree::Add(tree, -k);
		}
		else if (c == -1)
		{
			tree = AVLTree::Remove(tree, -k);
		}
		else
		{
			//cout << -AVLTree::GetKthMin(tree, k) << '\n';
			writeInt(-AVLTree::GetKthMin(tree, k), '\n');
		}
	}

	//_getch();
	return 0;
}