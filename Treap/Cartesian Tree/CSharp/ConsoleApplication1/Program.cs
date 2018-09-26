using System;
using System.Collections.Generic;

namespace ConsoleApplication1
{
    public class Treap
    {
        public int x;
        public int y;

        public Treap left;
        public Treap right;
        public Treap parent;

        private Treap(int x, int y, Treap left = null, Treap right = null, Treap parent = null)
        {
            this.x = x;
            this.y = y;
            this.left = left;
            this.right = right;
            this.parent = parent;
        }

        public static Treap Merge(Treap first, Treap second)
        {
            if (first == null)
            {
                return second;
            }
            if (second == null)
            {
                return first;
            }

            if (first.y > second.y)
            {
                return new Treap(first.x, first.y, first.left, Merge(first.right, second));
            }
            else
            {
                return new Treap(second.x, second.y, Merge(first, second.left), second.right);
            }

        }

        public void Split(int x0, out Treap first, out Treap second)
        {
            Treap newTreap = null;
            if (x <= x0)
            {
                if (right == null)
                {
                    second = null;
                }
                else
                {
                    right.Split(x0, out newTreap, out second);
                }
                first = new Treap(x, y, left, newTreap);
            }
            else
            {
                if (left == null)
                {
                    first = null;
                }
                else
                {
                    left.Split(x0, out first, out newTreap);
                }
                second = new Treap(x, y, newTreap, right);
            }
        }

        public Treap Remove(int x)
        {
            Treap l, m, r;
            Split(x - 1, out l, out r);
            r.Split(x, out m, out r);
            return Merge(l, r);
        }

        public static Treap Build(int[] xs, int[] ys)
        {
            var tree = new Treap(xs[0], ys[0]);
            var last = tree;

            for (int i = 1; i < xs.Length; ++i)
            {
                if (last.y > ys[i])
                {
                    last.right = new Treap(xs[i], ys[i], parent: last);
                    last = last.right;
                }
                else
                {
                    Treap cur = last;
                    while (cur.parent != null && cur.y <= ys[i])
                        cur = cur.parent;
                    if (cur.y <= ys[i])
                        last = new Treap(xs[i], ys[i], cur);
                    else
                    {
                        last = new Treap(xs[i], ys[i], cur.right, null, cur);
                        cur.right = last;
                    }
                }
            }

            while (last.parent != null)
                last = last.parent;
            return last;
        }
    }

    
    class Program
    {
        static void Main(string[] args)
        {
            int n = int.Parse(Console.ReadLine());
            int[] x = new int[n];
            int[] y = new int[n];
            for (int i = 0; i < n; i++)
            {
                string[] s = Console.ReadLine().Split(' ');
                x[i] = int.Parse(s[0]);
                //y[i] = int.Parse(s[1]);
                //xy.Add(x[i], -int.Parse(s[1]));
                //dict.Add(x[i], i + 1);
                //ans.Add(i + 1, new vertex { });
            }
            Array.Sort(x);
            /*for (int i = 0; i < n; i++)
            {
                y[i] = xy[x[i]];
            }*/
            Treap myTreap = Treap.Build(x, y);
           // Console.WriteLine("YES");
            DFS(myTreap, null);
           /* for (int i = 0; i < n; i++)
            {
                Console.WriteLine("{0} {1} {2}", ans[i + 1].parent, ans[i + 1].left, ans[i + 1].right);
            }*/
            //Console.ReadKey();
        }

        struct vertex
        {
            public int parent;
            public int left;
            public int right;
        }

        static Dictionary<int, int> dict = new Dictionary<int, int>();
        static Dictionary<int, int> xy = new Dictionary<int, int>();
        static Dictionary<int, vertex> ans = new Dictionary<int, vertex>();

        static void DFS(Treap current, Treap parent)
        {
            ans[dict[current.x]] = new vertex
            {
                parent = (parent == null ? 0 : dict[parent.x]),
                left = (current.left == null ? 0 : dict[current.left.x]),
                right = (current.right == null ? 0 : dict[current.right.x]),
            };
            if (current.left != null)
                DFS(current.left, current);
            if (current.right != null)
                DFS(current.right, current);
        }
    }
}
