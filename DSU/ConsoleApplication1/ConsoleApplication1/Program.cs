using System;
using System.IO;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            TextReader input = new StreamReader("cutting.in");
            TextWriter output = new StreamWriter("cutting.out");

            string[] s = input.ReadLine().Split(' ');
            int n = int.Parse(s[0]);
            int m = int.Parse(s[1]);
            int k = int.Parse(s[2]);

            for (int i = 0; i < m; i++)
            {
                s = input.ReadLine().Split(' ');
            }

            string[] queries = new string[k];
            for (int i = 0; i < k; i++)
            {
                queries[i] = input.ReadLine();
            }

            Reset(n + 1);

            string[] answer = new string[k];
            for (int i = k - 1; i >= 0; i--)
            {
                string[] currentQuerie = queries[i].Split(' ');
                if (currentQuerie[0] == "ask")
                {
                    int first = int.Parse(currentQuerie[1]);
                    int second = int.Parse(currentQuerie[2]);
                    if (CheckIfInOne(first, second))
                    {
                        answer[i] = "YES";
                    }
                    else
                    {
                        answer[i] = "NO";
                    }
                }
                else if (currentQuerie[0] == "cut")
                {
                    int first = int.Parse(currentQuerie[1]);
                    int second = int.Parse(currentQuerie[2]);
                    Union(first, second);
                }
            }

            for (int i = 0; i < k; i++)
            {
                if (answer[i] != null)
                {
                    output.WriteLine(answer[i]);
                }
            }
            output.Close();
        }

        static long[] parent;
        static long[] size;

        static void Reset(long newSize)
        {
            parent = new long[newSize];
            size = new long[newSize];

            for (long i = 0; i < newSize; i++)
            {
                parent[i] = i;
                size[i] = 1;
            }
        }

        static bool CheckIfInOne(long first, long second)
        {
            return (GetLeader(first) == GetLeader(second));
        }

        static long GetLeader(long element)
        {
            if (parent[element] == element)
                return element;

            long leader = GetLeader(parent[element]);
            parent[element] = leader;

            return leader;
        }

        static void Union(long first, long second)
        {
            long firstLeader = GetLeader(first);
            long secondLeader = GetLeader(second);

            if (firstLeader != secondLeader)
            {
                if (size[firstLeader] < size[secondLeader])
                {
                    long swapper = firstLeader;
                    firstLeader = secondLeader;
                    secondLeader = swapper;
                }
                if (size[firstLeader] == size[secondLeader])
                {
                    size[firstLeader]++;
                }
                parent[secondLeader] = firstLeader;
            }
        }
    }
}