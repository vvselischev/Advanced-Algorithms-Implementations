using System.Collections.Generic;
using System.IO;

namespace ConsoleApplication1
{
    class Program
    {
        public class Node
        {
            public List<int> patternId;
            public int[] next;
            public bool flag;
            public int link;
            public int superLink;
            public int parent;
            public char parentSymbol;
            public int[] nextMove;

            public Node(int link, char parentSymbol = '\0', bool flag = false, 
                int parent = -1, int superLink = -1)
            {
                patternId = new List<int>();
                this.link = link;
                this.flag = flag;
                next = new int[26];
                this.parent = parent;
                nextMove = new int[26];
                this.parentSymbol = parentSymbol;
                this.superLink = superLink;
                Reset();
            }
            private void Reset()
            {
                for (int i = 0; i < 26; i++)
                {
                    next[i] = -1;
                    nextMove[i] = -1;
                }
            }
        }

        static void Main(string[] args)
        {
            TextReader input = new StreamReader("search4.in");
            TextWriter output = new StreamWriter("search4.out");
            int n = int.Parse(input.ReadLine());
            string[] pattern = new string[n];
            for (int i = 0; i < n; i++)
            {
                pattern[i] = input.ReadLine();
            }
            string text = input.ReadLine();
            bohr = new List<Node>();
            bohr.Add(new Node(-1));

            for (int i = 0; i < n; i++)
            {
                AddString(pattern[i], i);
            }

            int current = 0;
            bool[] found = new bool[n];
            visited = new bool[bohr.Count + 1];
            for (int i = 0; i < text.Length; i++)
            {
                current = GetNext(current, text[i]);
                int vert = current;
                while (!visited[vert] && (vert != 0))
                {
                    visited[vert] = true;
                    if (bohr[vert].flag)
                    {
                        for (int j = 0; j < bohr[vert].patternId.Count; j++)
                        {
                            found[bohr[vert].patternId[j]] = true;
                        }
                    }
                    vert = GetSuperLink(vert);
                }
            }

            for (int i = 0; i < n; i++)
            {
                if (found[i])
                {
                    output.WriteLine("YES");
                }
                else
                {
                    output.WriteLine("NO");
                }
            }
            output.Close();
        }

        static int GetSuperLink(int vert)
        {
            if (bohr[vert].superLink == -1)
            {
                int next = GetLink(vert);
                if (next == 0)
                {
                    bohr[vert].superLink = 0;
                }
                else
                {
                    if (bohr[next].flag)
                    {
                        bohr[vert].superLink = next;
                    }
                    else
                    {
                        bohr[vert].superLink = GetSuperLink(next);
                    }
                }
            }
            return bohr[vert].superLink;
        }

        static bool[] visited;

        static int GetLink(int vert)
        {
            if (bohr[vert].link == -1)
            {
                if ((vert == 0) || (bohr[vert].parent == 0))
                {
                    bohr[vert].link = 0;
                }
                else
                {
                    bohr[vert].link = GetNext(GetLink(bohr[vert].parent),
                        bohr[vert].parentSymbol);
                }
            }
            return bohr[vert].link;
        }

        static int GetNext(int vert, char ch)
        {
            int symbol = ch - 'a';
            if (bohr[vert].nextMove[symbol] == -1)
            {
                if (bohr[vert].next[symbol] != -1)
                {
                    bohr[vert].nextMove[symbol] = bohr[vert].next[symbol];
                }
                else
                {
                    if (vert == 0)
                    {
                        bohr[vert].nextMove[symbol] = 0;
                    }
                    else
                    {
                        bohr[vert].nextMove[symbol] = GetNext(GetLink(vert), ch);
                    }
                }
            }
            return bohr[vert].nextMove[symbol];
        }

        static void AddString(string s, int id)
        {
            int current = 0;
            for (int i = 0; i < s.Length; i++)
            {
                int charId = s[i] - 'a';
                if (bohr[current].next[charId] == -1)
                {
                    bohr.Add(new Node(-1, parent: current, parentSymbol: s[i]));
                    bohr[current].next[charId] = bohr.Count - 1;
                }
                current = bohr[current].next[charId];
            }
            bohr[current].flag = true;
            bohr[current].patternId.Add(id);
        }

        static List<Node> bohr;
    }
}
