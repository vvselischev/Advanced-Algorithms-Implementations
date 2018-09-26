using System;
using System.Collections.Generic;
using System.IO;

namespace ConsoleApplication1
{
    class Program
    {
        public struct vector2
        {
            public long x;
            public long y;

            public static bool operator == (vector2 first, vector2 second)
            {
                return (first.x == second.x) && (first.y == second.y);
            }

            public static bool operator != (vector2 first, vector2 second)
            {
                return (first.x != second.x) || (first.y != second.y);
            }

            public static long CrossProduct(vector2 first, vector2 second)
            {
                return first.x * second.y - first.y * second.x;
            }

            public static vector2 operator - (vector2 first, vector2 second)
            {
                return new vector2
                {
                    x = first.x - second.x,
                    y = first.y - second.y,
                };
            }

            public decimal GetLength()
            {
                return (decimal)Math.Sqrt(x * x + y * y);
            }
        }

        static void Main(string[] args)
        {
            TextReader input = new StreamReader("hull.in");
            TextWriter output = new StreamWriter("hull.out");
            int n = int.Parse(input.ReadLine());
            vertex = new vector2[n];

            for (int i = 0; i < n; i++)
            {
                string[] s = input.ReadLine().Split(' ');
                vertex[i] = new vector2
                {
                    x = int.Parse(s[0]),
                    y = int.Parse(s[1]),
                };
                if ((vertex[i].x < vertex[0].x) || ((vertex[i].x == vertex[0].x) && 
                    (vertex[i].y < vertex[0].y)))
                {
                    vector2 swapper = vertex[i];
                    vertex[i] = vertex[0];
                    vertex[0] = swapper;
                }
            }

            Stack<vector2> hull = new Stack<vector2>();
            Array.Sort(vertex, new Comparison<vector2>(Comparator));
            hull.Push(vertex[0]);
            int last = 1;
            
            while (vertex[last] == vertex[0])
            {
                last++;
            }
            vector2 first = vertex[0];
            vector2 second = vertex[last];

            while (vector2.CrossProduct(vertex[last + 1] - second, second - first) == 0)
            {
                last++;
                second = vertex[last];
            }

            hull.Push(vertex[last]);
            first = vertex[0];
            second = vertex[last];
            vector2 lastEdge = second - first;
            for (int i = last + 1; i < n; i++)
            {
                while (vector2.CrossProduct(lastEdge, vertex[i] - second) <= 0)
                {
                    hull.Pop();
                    second = first;
                    hull.Pop();
                    first = hull.Peek();
                    hull.Push(second);
                    lastEdge = second - first;
                }
                hull.Push(vertex[i]);
                first = second;
                second = vertex[i];
                lastEdge = second - first;
            }

            output.WriteLine(hull.Count);
            List<vector2> hullVertex = new List<vector2>();
            int count = hull.Count;
            for (int i = 0; i < count; i++)
            {
                hullVertex.Add(hull.Peek());
                output.WriteLine("{0} {1}", hull.Peek().x, hull.Pop().y);
            }

            first = hullVertex[1];
            decimal square = 0;
            for (int i = 2; i < hullVertex.Count; i++)
            {
                decimal currentSquare =  vector2.CrossProduct(hullVertex[i] - hullVertex[0], 
                    first - hullVertex[0]) / (decimal)2;
                square += currentSquare;
                first = hullVertex[i];
            }

            output.WriteLine(square.ToString().Replace(',', '.'));
            output.Close();
        }

        static vector2[] vertex;

        static int Comparator(vector2 first, vector2 second)
        {
            if (first == vertex[0])
            {
                return -1;
            }
            if (second == vertex[0])
            {
                return 1;
            }
            vector2 firstEdge = first - vertex[0];
            vector2 secondEdge = second - vertex[0];
            long crossProduct = vector2.CrossProduct(firstEdge, secondEdge);
            if (crossProduct != 0)
            {
                return -Math.Sign(crossProduct);
            }
            return Math.Sign(firstEdge.GetLength() - secondEdge.GetLength());
        }
    }
}
