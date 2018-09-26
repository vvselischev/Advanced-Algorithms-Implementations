package com.company;

import java.io.*;
import java.util.*;

public class Main
{
    public void run() throws IOException
    {
        FastScanner input = new FastScanner(new File("search4.in"));
        PrintWriter output = new PrintWriter(new File("search4.out"));

        int n = input.nextInt();
        String[] pattern = new String[n];
        for (int i = 0; i < n; i++)
        {
            pattern[i] = input.next();
        }
        String text = input.next();
        bohr = new ArrayList<>();
        bohr.add(new Node(-1, '\0', false, -1, -1));

        for (int i = 0; i < n; i++)
        {
            AddString(pattern[i], i);
        }

        int current = 0;
        boolean[] found = new boolean[n];
        visited = new boolean[bohr.size()];
        for (int i = 0; i < text.length(); i++)
        {
            current = GetNext(current, text.charAt(i));
            int vert = current;
            while ((vert != 0) && (!visited[vert]))
            {
                visited[vert] = true;
                if (bohr.get(vert).flag)
                {
                    for (int j = 0; j < bohr.get(vert).patternId.size(); j++)
                    {
                        found[bohr.get(vert).patternId.get(j)] = true;
                    }
                }
                vert = GetSuperLink(vert);
            }
        }

        for (int i = 0; i < n; i++)
        {
            if (found[i])
            {
                output.println("YES");
            }
            else
            {
                output.println("NO");
            }
        }
        output.close();
    }

    boolean[] visited;

    int GetSuperLink(int vert)
    {
        if (bohr.get(vert).superLink == -1)
        {
            int next = GetLink(vert);
            if (next == 0)
            {
                bohr.get(vert).superLink = 0;
            }
            else
            {
                if (bohr.get(next).flag)
                {
                    bohr.get(vert).superLink = next;
                }
                else
                {
                    bohr.get(vert).superLink = GetSuperLink(next);
                }
            }
        }
        return bohr.get(vert).superLink;
    }

    int GetLink(int vert)
    {
        if (bohr.get(vert).link == -1)
        {
            if ((vert == 0) || (bohr.get(vert).parent == 0))
            {
                bohr.get(vert).link = 0;
            }
            else
            {
                bohr.get(vert).link = GetNext(GetLink(bohr.get(vert).parent),
                        bohr.get(vert).parentSymbol);
            }
        }
        return bohr.get(vert).link;
    }

    int GetNext(int vert, char ch)
    {
        int symbol = ch - 'a';
        if (bohr.get(vert).nextMove[symbol] == -1)
        {
            if (bohr.get(vert).next[symbol] != -1)
            {
                bohr.get(vert).nextMove[symbol] = bohr.get(vert).next[symbol];
            }
            else
            {
                if (vert == 0)
                {
                    bohr.get(vert).nextMove[symbol] = 0;
                }
                else
                {
                    bohr.get(vert).nextMove[symbol] = GetNext(GetLink(vert), ch);
                }
            }
        }
        return bohr.get(vert).nextMove[symbol];
    }

    void AddString(String s, int id)
    {
        int current = 0;
        for (int i = 0; i < s.length(); i++)
        {
            int charId = s.charAt(i) - 'a';
            if (bohr.get(current).next[charId] == -1)
            {
                bohr.add(new Node(-1, s.charAt(i), false, current, -1));
                bohr.get(current).next[charId] = bohr.size() - 1;
            }
            current = bohr.get(current).next[charId];
        }
        bohr.get(current).flag = true;
        bohr.get(current).patternId.add(id);
    }

    ArrayList<Node> bohr;

    public static void main(String[] args)
    {
        try
        {
            new Main().run();
        } catch (IOException e)
        {
            e.printStackTrace();
            System.exit(42);
        }
    }

    class Node
    {
        public List<Integer> patternId;
        public int[] next;
        public boolean flag;
        public int link;
        public int superLink;
        public int parent;
        public char parentSymbol;
        public int[] nextMove;

        public Node(int link, char parentSymbol, boolean flag,
                    int parent, int superLink)
        {
            patternId = new ArrayList<>();
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

    class FastScanner {
        BufferedReader reader;
        StringTokenizer tokenizer;

        FastScanner(File file) throws FileNotFoundException
        {
            reader = new BufferedReader(new FileReader(file));
            tokenizer = null;
        }

        String next() throws IOException
        {
            while (tokenizer == null || !tokenizer.hasMoreTokens())
            {
                tokenizer = new StringTokenizer(reader.readLine());
            }
            return tokenizer.nextToken();
        }

        int nextInt() throws IOException
        {
            return Integer.parseInt(next());
        }
    }
}
