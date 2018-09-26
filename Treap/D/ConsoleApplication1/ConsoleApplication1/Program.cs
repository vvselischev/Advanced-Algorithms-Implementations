using System;
using System.IO;

namespace ConsoleApplication1
{
    public class ImplicitTreap
    {
        public double y;
        public long value;
        private long size;
        public long min;
        public ImplicitTreap left;
        public ImplicitTreap right;

        public ImplicitTreap(double y, long value, ImplicitTreap left = null, ImplicitTreap right = null)
        {
            this.y = y;
            this.left = left;
            this.right = right;
            this.value = value;
            size = 1;
            min = long.MaxValue;
            Recalc();
        }

        public static long GetSize(ImplicitTreap treap)
        {
            if (treap == null)
            {
                return 0;
            }
            return treap.size;
        }


        public void Recalc()
        {
            size = GetSize(left) + GetSize(right) + 1;
            min = Math.Min(left == null ? long.MaxValue : left.value, 
                  Math.Min(right == null ? long.MaxValue : right.value, value));
        }

        public static ImplicitTreap Merge(ImplicitTreap first, ImplicitTreap second)
        {
            //Push(first);
            //Push(second);

            if (first == null)
            {
                return second;
            }
            if (second == null)
            {
                return first;
            }

            ImplicitTreap newTreap;
            if (first.y > second.y)
            {
                newTreap = new ImplicitTreap(first.y, first.value, first.left, Merge(first.right, second));
            }
            else
            {
                newTreap = new ImplicitTreap(second.y, second.value, Merge(first, second.left), second.right);
            }

            newTreap.Recalc();
            return newTreap;
        }

        public void Split(long x0, out ImplicitTreap first, out ImplicitTreap second)
        {
            //Push(this);

            ImplicitTreap newTreap = null;
            if (GetSize(left) + 1 > x0)
            {
                if (left == null)
                {
                    first = null;
                }
                else
                {
                    left.Split(x0, out first, out newTreap);
                }
                second = new ImplicitTreap(y, value, newTreap, right);
                second.Recalc();
            }
            else
            {
                if (right == null)
                {
                    second = null;
                }
                else
                {
                    right.Split(x0 - GetSize(left) - 1, out newTreap, out second);
                }
                first = new ImplicitTreap(y, value, left, newTreap);
                first.Recalc();
            }
        }

        /*public ImplicitTreap Add(long value, long pos, long y)
        {
            ImplicitTreap first, second, middle;
            Split(pos, out first, out second);
            middle = new ImplicitTreap(y, value);
            return Merge(Merge(first, middle), second);
        }*/

        //public bool reversed;

        /*public static void Push(ImplicitTreap treap)
        {
            if (treap == null) return;
            if (!treap.reversed) return;

            var temp = treap.left;
            treap.left = treap.right;
            treap.right = temp;

            treap.reversed = false;
            if (treap.left != null) treap.left.reversed ^= true;
            if (treap.right != null) treap.right.reversed ^= true;
        }*/

        /*public ImplicitTreap Reverse(int A, int B) 
        {
            ImplicitTreap l, m, r;
            Split(A - 1, out l, out r);
            r.Split(B - A + 1, out m, out r);
            m.reversed ^= true;
            return Merge(Merge(l, m), r);
        }*/
    }

    class Program
    {
        static void Main(string[] args)
        {
            TextReader input = new StreamReader("input.txt");
            TextWriter output = new StreamWriter("output.txt");

            int n = int.Parse(input.ReadLine());
            ImplicitTreap treap = null;
            Random rnd = new Random();

            for (int i = 0; i < n; i++)
            {
                string[] s = input.ReadLine().Split(' ');
                if (s[0][0] == '+')
                {
                    int x0 = int.Parse(s[1]);
                    int arg = int.Parse(s[2]);
                    ImplicitTreap tmp = new ImplicitTreap(rnd.NextDouble(), arg);
                    if (x0 == 0)
                    {
                        treap = ImplicitTreap.Merge(tmp, treap); 
                    }
                    else
                    {
                        ImplicitTreap first, second;
                        treap.Split(x0, out first, out second);
                        treap = ImplicitTreap.Merge(ImplicitTreap.Merge(first, tmp), second);
                    }
                }
                else
                {
                    int l = int.Parse(s[1]);
                    int r = int.Parse(s[2]);
                    ImplicitTreap first, second, middle;
                    treap.Split(l - 1, out first, out second);
                    second.Split(r - l + 1, out middle, out second);
                    middle.Recalc();
                    output.WriteLine(middle.min);
                }
            }
            //Console.ReadKey();
            output.Close();

            /*string[] s = Console.ReadLine().Split(' ');
            int n = int.Parse(s[0]);
            int m = int.Parse(s[1]);

            s = Console.ReadLine().Split(' ');
            Random rnd = new Random();
            ImplicitTreap treap = new ImplicitTreap(rnd.Next(), int.Parse(s[0]));
            for (int i = 1; i < n; i++)
            {
                treap = ImplicitTreap.Merge(treap, new ImplicitTreap(rnd.Next(), int.Parse(s[i])));
            }

            for (int i = 0; i < m; i++)
            {
                s = Console.ReadLine().Split(' ');
                if (s[0] == "2")
                {
                    int l = int.Parse(s[1]);
                    int r = int.Parse(s[2]);
                    ImplicitTreap first, second, middle;
                    treap.Split(l - 1, out first, out second);
                    second.Split(r - l + 1, out middle, out second);
                    //middle.Recalc();
                    Console.WriteLine(middle.min);
                }
                else
                {
                    int l = int.Parse(s[1]);
                    int r = int.Parse(s[2]);
                    treap = treap.Reverse(l, r);
                }
            }*/
            //Console.ReadKey();
        }
    }
}
