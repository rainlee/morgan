/*
 Jumping Genes
 
 A secret agency involved in extra-terrestrial intelligence found tissue samples
 of a creature presumably with extra-terrestrial origins. They realized that it 
 contains a DNA composed of not 4, but 26 different nucleobases.  
 The agency is now searching for special DNA sequences called transposons (also 
 known as jumping genes) that are able to change their positions within the genome. 
 The way some of these DNA sequences can cut themselves out of the genome (before 
 they paste themselves back in somewhere else) is by containing an inverted repeat
 at the boundaries of the transposon. An inverted repeat is a substring and its 
 reverse complement, so that between the substring and the reverse complement there
 can be any number of nucleobases including zero. Because complementing sequences 
 tend to stick together, transposons can stick their inverted repeats together at 
 their boundaries (forming a loop) making sure to join the DNA sequence ends that 
 they leave behind when they cut themselves out.
  
 Your task is to help the agency find transposons by locating the longest inverted 
 repeat within given DNA sequence, i.e. a substring and its reverse complement present 
 anywhere in the DNA sequence (but without overlap). Because mutations sometimes 
 can occur, we allow the reverse complement (but only the reverse complement!) to be 
 present with the possible inclusion of up to 2 extra nucleotides.
 
 Input format 
 The 26 nucleobases are represented by upper-case letters of the English alphabet. 
 Each nucleobase is at a distance of 13 letters from its complement in the English 
 alphabet (e.g. A is a complement of N and vice versa, and R is a complement of 
 E and vice versa). Each test case contains a single string describing the DNA to 
 examine that is not longer than 20000 nucleotides.
 
 Output format 
 You need to specify 3 lines of output containing values SEQUENCE, FW_POS, BW_POS respectively.
 SEQUENCE should contain the longest sequence in the DNA that also has a reverse
 complement within the input. FW_POS must be a positive integer containing the starting
 index of SEQUENCE within the DNA. BW_POS is a positive integer pointing to the beginning 
 of the reverse complement of the sequence. 1 denotes the first nucleotide of the DNA.
 When no transposons are found, a single line containing a dash (‘-‘) must be specified.
 
 Sample input #1 
 AIBICXYPON
 
 Sample output #1 
 PON
 8
 1
 
 Sample input #2 
 ABN
 
 Sample output #2 
 A
 1
 3
 
 Sample input #3 
 ABCD
 
 Sample output #3 
 - 
 */

/***
 * 题意如下：
 * DNS碱基对，两两一配对，中间间隔13，如A-N, B-O
 * 现在要求最长的连续配对数，可以有2个变异（不匹配）
 * 就是求最长回文子串（允许两次变异）
 * 先求出原串的配对串，然后求两个字符串的相同连续最大逆序子串
 ***/
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

void JumpGenes(string &so)
{
    int n = so.size();
    string sc(n, 'A');

    for (int i = 0; i < n; ++i)
    {
        if (so[i] <= 'M')  // A-N, 以M为界
            sc[i] = (char)(so[i] + 13);
        else
            sc[i] = (char)(so[i] - 13);
    }

    int maxlen = 0;
    int fw_pos = -1;
    int bw_pos = -1;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (so[i] == sc[j])
            {
                int cnt = 1;
                if (cnt > maxlen)
                {
                    maxlen = cnt;
                    fw_pos = i;
                    bw_pos = j;
                }

                int i2 = i + 1;
                int j2 = j - 1;
                int f = 2;       // 允许2个变异
                while (f >= 0)
                {
                    if ((i2 < 0) || (j2 >= n))
                        break;
                    if (so[i2] == sc[j2])
                    {
                        cnt++;   // 又匹配了一个
                        i2++;

                        if (cnt > maxlen)
                        {
                            maxlen = cnt;
                            fw_pos = i;
                            bw_pos = j2;
                        }
                    }
                    else
                        f--;
                    j2--;
                } // while
            } // if
        } // for
    } // for

    // output
    if (fw_pos == -1)
        cout << "-" << endl;
    else
    {
        cout << so.substr(fw_pos, maxlen) << endl;
        cout << (fw_pos + 1) << endl;
        cout << (bw_pos + 1) << endl;
    }
}

int main()
{
    freopen("input21.txt", "r", stdin);
    freopen("output21.txt", "w", stdout);

    string s;
    while (cin >> s)
    {
        if (s != ";")
            JumpGenes(s);
        else
            cout << ";" << endl;
    }
    return 0;
}
