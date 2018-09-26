using System;
//using System.Collections.Generic;
using System.IO;

namespace ConsoleApplication1
{
    public class Treap
    {
        public long x;
        public double y;
        public long min;

        public Treap Left;
        public Treap Right;

        public Treap(long x, double y, Treap left = null, Treap right = null)
        { 
            this.x = x;
            this.y = y;
            this.Left = left;
            this.Right = right;
            min = x;
            Recalc();
        }

        public void Recalc()
        {
            if (Left == null)
            {
                if (Right != null)
                {
                    min = Math.Min(min, Right.min); 
                }
            }
            else
            {
                if (Right == null)
                {
                    min = Math.Min(min, Left.min);
                }
                else
                {
                    min = Math.Min(Left.min, Math.Min(min, Right.min));
                }
            }
        }

        public static Treap Merge(Treap L, Treap R) 
        {
            if (L == null) return R;
            if (R == null) return L;

           // TreapnewT;
            if (L.y > R.y)
            {
                return new Treap(L.x, L.y, L.Left, Merge(L.Right, R));
            }
            else
            {
                return new Treap(R.x, R.y, Merge(L, R.Left), R.Right);
            }
        }

        public void Split(long x, out Treap L, out Treap R)
        {
            Treap newTree = null;
            if (this.x <= x)
            {
                if (Right == null)
                    R = null;
                else
                    Right.Split(x, out newTree, out R);
                L = new Treap(this.x, y, Left, newTree);
            }
            else
            {
                if (Left == null)
                    L = null;
                else
                    Left.Split(x, out L, out newTree);
                R = new Treap(this.x, y, newTree, Right);
            }
        }
    }


    class Program
    {
        static void Main(string[] args)
        {
            TextReader input = Console.In; //new StreamReader("input.txt");
            TextWriter output = new StreamWriter("output.txt");

            int n = int.Parse(input.ReadLine());
            rnd = new Random();
            char prev = 'q';  
            long lastAns = 0;
            string[] s = null;
            char type = 'w';
            long arg = -2;
            for (int i = n; --i >= 0;)
            {
                s = input.ReadLine().Split(' ');
                type = s[0][0];
                arg = long.Parse(s[1]);
                
                if (type == '+')
                {
                    if ((prev == '+') || (prev == 'q'))
                    {
                        AddUnique(arg);
                    }
                    else
                    {
                        AddUnique((arg + lastAns) % 1000000000);
                    }
                }
                else
                {
                    lastAns = Next(arg);
                    output.WriteLine(lastAns);
                }
                prev = type;
            }
            //Console.ReadKey();
            output.Close();
        }

        static Treap treap;
        static Random rnd;

        static long Next(long x0)
        {
            Treap first, second;
            treap.Split(x0 - 1, out first, out second);

            long ans = -1;
            if (second != null)
            {
                ans = second.min;
            }
            return ans;
        }    
         
        static void AddUnique(long x)
        {
            if (treap == null)
            {
                treap = new Treap(x, rnd.NextDouble());
            }
            else
            {
                Treap first, second, middle;
                treap.Split(x - 1, out first, out second);

                if (second != null)
                {
                    second.Split(x, out middle, out second);
                    if (middle == null)
                    {
                        middle = new Treap(x, rnd.NextDouble());
                        treap = Treap.Merge(Treap.Merge(first, middle), second);
                    }
                }
                else
                {
                    middle = new Treap(x, rnd.NextDouble());
                    treap = Treap.Merge(first, middle);
                } 
            }
        }
    }
}
