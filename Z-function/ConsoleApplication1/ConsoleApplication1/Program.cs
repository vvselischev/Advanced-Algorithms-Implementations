using System;
using System.Collections.Generic;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            string s = Console.ReadLine();
            int n = s.Length;
            int[] z = new int[n];
            int l = 0;
            int r = 0;
            z[0] = n;
            for (int i = 1; i < n; i++)
            {
                if (i <= r)
                {
                    z[i] = Math.Min(z[i - l], r - i + 1);
                }
                while ((i + z[i] < n) && (s[i + z[i]] == s[z[i]]))
                {
                    z[i]++;
                }
                if (i + z[i] - 1 > r)
                {
                    l = i;
                    r = i + z[i] - 1;
                }
            }
                

            for (int i = 0; i < n; i++)
            {
                Console.Write(z[i].ToString() + " ");
            }
            //Console.ReadKey();
        }
    }
}
