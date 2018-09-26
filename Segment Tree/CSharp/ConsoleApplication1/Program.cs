using System;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            int n = int.Parse(Console.ReadLine());

            string[] s = Console.ReadLine().Split(' ');
            int[] a = new int[n + 1];
            tree = new long[4 * n + 2];

            for (int i = 1; i <= n; i++)
            {
                a[i] = int.Parse(s[i - 1]);
            }
            Build(a, 1, 1, n);


            int q = int.Parse(Console.ReadLine());
            for (int i = 0; i < q; i++)
            {
                s = Console.ReadLine().Split(' ');
                int left = int.Parse(s[0]);
                int right = int.Parse(s[1]);
                long ans = Find(1, n, left, right);
                Console.WriteLine(ans);
            }
            
            Console.ReadKey();
        }

        static long[] tree;

        static void Build(int[] a, int current, int left, int right)
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

        static long Find(int current, int count, int left, int right)
        {
            if (right - left + 1 == count)
                return tree[current];

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

        static long makeParent(long a, long b)
        {
            return Math.Max(a, b);
        }
    }
}
