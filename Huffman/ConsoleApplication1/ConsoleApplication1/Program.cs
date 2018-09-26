using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Collections;
using System.Text;

namespace ConsoleApplication1
{
    class Program
    {
        struct CodePair
        {
            public char ch;
            public string code;
        }

        class Node
        {
            public char ch;
            public Node left;
            public Node right;
            public readonly int count;

            public bool IsLeaf() => (left == null) && (right == null);

            public Node(int count)
            {
                this.count = count;
            }
        }

        static IEnumerable GetStatKeys(string s, out Dictionary<char, int> stat)
        {
            stat = new Dictionary<char, int>();
            foreach (char c in s)
            {
                if (!stat.ContainsKey(c))
                {
                    stat.Add(c, 0);
                }
                stat[c]++;
            }
            return stat.OrderByDescending(obj => obj.Value).Select(obj => obj.Key);
        }

        static void PrintStat(string s, TextWriter output)
        {
            Dictionary<char, int> stat;
            var enumerableKeys = GetStatKeys(s, out stat);
            foreach (char c in enumerableKeys)
            {
                output.WriteLine(c + ": " + stat[c]);
            }
        }

        static void PrintCode(Dictionary<char, string> code, TextWriter output)
        {
            foreach (char c in code.Keys)
            {
                output.WriteLine(c + ": " + code[c]);
            }
        }

        static IEnumerable<CodePair> GetLeaves(Node node, string currentString)
        {
            if (node.IsLeaf())
            {
                yield return new CodePair()
                {
                    ch = node.ch,
                    code = currentString
                };
            }
            else
            {
                foreach (var leaf in GetLeaves(node.left, currentString + "0"))
                {
                    yield return leaf;
                }
                foreach (var leaf in GetLeaves(node.right, currentString + "1"))
                {
                    yield return leaf;
                }
            }
        }

        static Dictionary<char, string> GetHuffmanCode(string s)
        {
            Dictionary<char, int> stat;
            SortedList<int, Queue<Node>> nodes =
                new SortedList<int, Queue<Node>>();
            List<char> keys = GetStatKeys(s, out stat).Cast<char>().ToList();

            int count = keys.Count;
            if (count == 1)
            {
                Dictionary<char, string> code = new Dictionary<char, string> { { s[0], "0" } };
                return code;
            }

            foreach (char c in keys)
            {
                if (!nodes.ContainsKey(stat[c]))
                    nodes.Add(stat[c], new Queue<Node>());
                nodes[stat[c]].Enqueue(new Node(stat[c]) {ch = c});
            }

            while (count >= 2)
            {
                Queue<Node> q = nodes.Values[0];
                Node first = q.Dequeue();
                if (q.Count == 0)
                {
                    nodes.RemoveAt(0);
                }
                q = nodes[nodes.Keys[0]];
                Node second = q.Dequeue();
                if (q.Count == 0)
                {
                    nodes.RemoveAt(0);
                }

                Node parent = new Node(first.count + second.count)
                {
                    left = first,
                    right = second,
                };
                if (!nodes.ContainsKey(parent.count))
                {
                    nodes.Add(parent.count, new Queue<Node>());
                }
                nodes[parent.count].Enqueue(parent);
                count--;
            }

            Node startNode = nodes[nodes.Keys[0]].Dequeue();
            return GetLeaves(startNode, "").ToDictionary(pair => pair.ch, pair => pair.code);
        }

        static string CodeHuffman(Dictionary<char, string> code, string s)
        {
            return s.Aggregate("", (current, c) => current + code[c]);
        }

        static string DecodeHuffman(Dictionary<char, string> code, string s)
        {
            Dictionary<string, char> invertedCode = code.Keys.ToDictionary(c => code[c]);
            string curString = "";
            string ans = "";
            foreach (char c in s)
            {
                curString += c;
                if (invertedCode.ContainsKey(curString))
                {
                    ans += invertedCode[curString];
                    curString = "";
                }
            }
            return ans;
        }

        const int blockSize = 8;
        static string CompressCodedString(string codedString)
        {
            string compressed = "";
            int position = 0;
            int charCode;
            string substr;

            while (position + blockSize < codedString.Length)
            {
                substr = codedString.Substring(position, blockSize);
                charCode = Convert.ToInt32(substr, 2);
                compressed += (char)charCode;
                position += blockSize;
            }

            //в конце пишем размер последнего блока, т.к. иначе не поймём, 
            //сколько ведущих нулей добавлять при декодировании
            substr = codedString.Substring(position, codedString.Length - position);
            charCode = Convert.ToInt32(substr, 2);
            compressed += (char)charCode;
            compressed += codedString.Length - position;
            return compressed;
        }

        static string DecompressCodedString(string compessedString)
        {
            string decompressed = "";
            foreach (char c in compessedString.Take(compessedString.Length - 2))
            {
                decompressed += Convert.ToString(c, 2).PadLeft(blockSize, '0');
            }
            char compressedChar = compessedString[compessedString.Length - 2];
            int lastSubstrLength = compessedString[compessedString.Length - 1] - '0';
            decompressed += Convert.ToString(compressedChar, 2).PadLeft(lastSubstrLength, '0');
            return decompressed;
        }

        static void Main(string[] args)
        {
            TextReader input = new StreamReader("t.txt", Encoding.Unicode);
            TextReader inputTest = new StreamReader("GettAdress.txt", Encoding.Unicode);
            TextWriter output = new StreamWriter("testOut.txt", false, Encoding.Unicode);
            string s = input.ReadToEnd();
            string s1 = inputTest.ReadToEnd();
            
            Dictionary<char, string> code = GetHuffmanCode(s);
            string codedString = CodeHuffman(code, s1);
            string decodedString = DecodeHuffman(code, codedString);
            string compressedCodedString = CompressCodedString(codedString);
            string decompressedCodedString = DecompressCodedString(compressedCodedString);
            string decodedDecompressedString = DecodeHuffman(code, decompressedCodedString);


            output.WriteLine(compressedCodedString);
            //output.WriteLine(DecodeHuffman(code, DecompressCodedString(s1)));

            //PrintCode(code, output);
            //PrintStat(s, output);
            /*output.WriteLine(s);
            output.WriteLine();
            PrintStat(s, output);
            output.WriteLine();
            PrintCode(code, output);
            output.WriteLine();
            output.WriteLine(codedString);
            output.WriteLine();*/
            //output.WriteLine(compressedCodedString);
            

            output.Close();
        }
    }
}