package com.company;

import java.io.*;
import java.util.*;

public class Main
{
    public static void main(String[] args) {
        try {
            new Main().run();
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(42);
        }
    }
    static Scanner scanner = new Scanner(System.in);
    public void run() throws IOException
    {
        sa_init();

        //int n = scanner.nextInt();
        //for (int i = 0; i < n; i++)
        String pattern = scanner.next();
        {
            String s = scanner.next();
            for (int j = 0; j < s.length(); j++)
                sa_extend(s.charAt(j));
            //sa_extend('.');
        }
        //sa_make_cnt();


            int count = sa_cnt(pattern);
            if (count == 1)
                System.out.println("yes");
            else
                System.out.println("no");
    }

    static final int MAXLEN = 600100;
    static state[] st = new state[MAXLEN*2];
    static int sz;
    static int last;
    static int[] cnt = new int[MAXLEN*2];

    void sa_init() throws IOException
    {
        sz = 0;
        last = 0;
        st[0] = new state(0, -1, new TreeMap<Character, Integer>());
        cnt[0] = 1;
        sz++;
    }

    void sa_extend (char c)
    {
        int cur = sz++;
        if (st[cur] == null)
            st[cur] = new state(0, 0, new TreeMap<Character, Integer>());
        st[cur].len = st[last].len + 1;
        int p;
        for (p = last; (p != -1) && (!st[p].next.containsKey(c)); p = st[p].link)
            st[p].next.put(c, cur);
        if (p == -1)
            st[cur].link = 0;
        else {
            int q = st[p].next.get(c);
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
                cnt[cur] = 1;
            }
            else {
                int clone = sz++;
                st[clone] = new state(st[p].len + 1, st[q].link, st[q].next); //
                for (; (p != -1) && (st[p].next.get(c) == q); p = st[p].link)
                {
                    st[p].next.put(c, clone);
                    int tmp = st[p].link;
                    if (tmp < 0) break;
                    if (st[tmp] == null)
                        break;
                    if (!st[tmp].next.containsKey(c)) break;
                }
                st[q].link = clone;
                st[cur].link = clone;
            }
        }
        last = cur;
    }

    static void sa_make_cnt()
    {
        //Arrays.sort(st, 0, sz, new SortedByLength());
        for (int i = 0; i < sz; i++)
        {
            if (st[i].link != -1)
                cnt[st[i].link] += cnt[i];
        }
    }

    static int sa_cnt(String pattern)
    {
        //int v = sz - 1;
        int v = 0;
        int ind = 0;
        while (true)
        {
            if (st[v].next.containsKey(pattern.charAt(ind)))
            {
                v = st[v].next.get(pattern.charAt(ind));
                ind++;
                if (ind == pattern.length())
                    //return cnt[v];
                    return 1;
            }
            else
                return 0;
        }
    }

    class state
    {
        public int len;
        public int link;
        public Map<Character,Integer> next;
        public state getInstance(int len, int link, Map<Character,Integer> next)
        {
            return new state(len, link, next);
        }
        state(int len1, int link1, Map<Character,Integer> next1)
        {
            len = len1;
            link = link1;
            next = next1;
        }
        state() throws FileNotFoundException
        {

        }
    }

    class SortedByLength implements Comparator<state>
    {
        public int compare(state obj1, state obj2)
        {
            return obj2.len - obj1.len;
        }
    }
}
