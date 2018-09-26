using System;
using System.Collections.Generic;
using System.IO;

namespace ConsoleApplication1
{
    class Program
    {
        public struct Edge
        {
            public int first;
            public int second;
            public int weight;
        }

        static void Main(string[] args)
        {
            TextReader input = new StreamReader("input.txt");
            TextWriter output = new StreamWriter("output.txt");

            string[] s = input.ReadLine().Split(' ');
            int n = int.Parse(s[0]);
            int m = int.Parse(s[1]);
            List<Edge> edges = new List<Edge>();
            Reset(n + 1);

            for (int i = 0; i < m; i++)
            {
                s = input.ReadLine().Split(' ');
                edges.Add(new Edge
                {
                    first = int.Parse(s[0]),
                    second = int.Parse(s[1]),
                    weight = int.Parse(s[2]),
                });
            }

            edges.Sort((first, second) => (first.weight - second.weight));

            long totalWeight = 0;
            for (int i = 0; i < m; i++)
            {
                Edge currentEdge = edges[i];
                int first = currentEdge.first;
                int second = currentEdge.second;

                if (!CheckIfInOne(first, second))
                {
                    totalWeight += currentEdge.weight;
                    Union(first, second);
                }
            }

            output.WriteLine(totalWeight);
            output.Close();
        }

        static int[] parent;
        static int[] depth;

        static void Reset(int size)
        {
            parent = new int[size];
            depth = new int[size];

            for (int i = 0; i < size; i++)
            {
                parent[i] = i;
                depth[i] = 1;
            }
        }

        static bool CheckIfInOne(int first, int second)
        {
            return GetLeader(first) == GetLeader(second);
        }

        static int GetLeader(int element)
        {
            if (parent[element] == element)
            {
                return element;
            }

            int leader = GetLeader(parent[element]);
            parent[element] = leader;
            return leader;
        }

        static void Union(int first, int second)
        {
            int firstLeader = GetLeader(first);
            int secondLeader = GetLeader(second);

            if (depth[first] < depth[second])
            {
                int swapper = firstLeader;
                firstLeader = secondLeader;
                secondLeader = swapper;
            }

            parent[secondLeader] = firstLeader;
            if (depth[firstLeader] == depth[secondLeader])
            {
                depth[firstLeader]++;
            }
        }
    }
}
