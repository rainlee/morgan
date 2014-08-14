/*
 Card trick
 
 A magician has prepared a deck of cards for the following card trick. 
 He plans to give the prepared full deck to someone from the audience, 
 and starts picking cards one by one, seemingly in a random fashion. 
 After he has chosen a card, he instantly tells the spectator to put it 
 either to the top or the bottom of the deck. He repeats this until the deck 
 gets fully reorganized into a designated state.
 
 Help the magician to re-shuffle the deck with as few moves as possible!
 
 Input format 
 The first line contains the description of the starting deck from bottom to top.
 Each card is represented by an integer – the higher the value, the closer 
 its designated position is to the top of the deck. These numbers are less 
 than 2147483648 and are separated by whitespace. The number of cards in the 
 deck is never higher than 60000.
 
 Output format 
 The first line should contain an integer N denoting the least number of required 
 moves to re-order the whole deck. 
 N lines follow, containing the ordered sequence of required moves. 
 Moves are described by a card identifier followed by letter B or T 
 depending whether the card should be placed to the bottom or top of the deck. 
 Card identifiers are unique. Multiple solutions might exist with the same N number 
 of steps; you may provide any of those.
 
 Sample input 
 3 5 7 1 9 8
 
 Sample output 
 2
 1B
 9T
*/

/***
 * 先对org数组排序，存在另一个sort数组中
 * 用一个辅助数组inline，记录顺序数组的长度
 * 根据第i个在sort数组中的位置，inline[i] = inline[i-1] + 1
 * 例如：
 * org：   3 2 4 1 9 8
 * sort：  1 2 3 4 8 9
 * inline：1 1 1 2 3 1
 * 3是第一个，所以顺序数组为1；2前面的1没有排，长1；4前的3已经排好，长2
 * 1应该排在首位，长1；9前面的8没排，长1；8前面的4排了，长3
 * 可以看到，顺序数组[3 4 8]，前后都是逆序（所有为1的都是逆序）
 * 前半部分逆序bottom，后半部分顺序top
 ***/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;

#define LEE_TEST

int biSearch(vector<int> &vnum, int target)
{
    int start = 0;
    int end = vnum.size() - 1;
    while (start < end)
    {
        int mid = start + (end - start) / 2;
        if (target == vnum[mid])
            return mid;
        else if (target > vnum[mid])
            start = mid + 1;
        else
            end = mid - 1;
    }
    if (vnum[start] < target)
        return start+1;
    else
        return start;
}

void vprint(vector<int> &vnum)
{
    for (int i = 0; i < vnum.size(); ++i)
    {
        if (i > 0)
            cout << " ";
        cout << vnum[i];
    }
    cout << endl;
}

void cardTrick(vector<int> &vorg, vector<int> &vsort)
{
    int n = vorg.size();
    if (n == 0)
    {
        cout << "0" << endl;
        return;
    }

    sort(vsort.begin(), vsort.end());    // 先排序

    vector<int> vinline(n, 0);

    int pos = biSearch(vsort, vorg[0]);  // 第一个顺序长度为1
    vinline[pos] = 1;

    for (int i = 1; i < n; ++i)   // 模拟排序
    {
        pos = biSearch(vsort, vorg[i]);
        if (pos > 0)
            vinline[pos] = vinline[pos - 1] + 1;  // 排在pos位置的顺序长度是前面已排的顺序长度+1
        else
            vinline[0] = 1;
    }

    //vprint(vorg);
    //vprint(vsort);
    //vprint(vinline);

    int max_inline = 0;        // 找到最长顺序
    int max_pos = 0;
    for (int i = 0; i < n; ++i)
    {
        if (vinline[i] > max_inline)
        {
            max_inline = vinline[i];
            max_pos = i;
        }
    }

    int sort_num = n - max_inline;  // 需要的排序次数 = 总长度 - 有序长度
    cout << sort_num << endl;

    // 排前半部分，逆向bottom
    for (int i = max_pos - max_inline; i >= 0; --i)
        cout << vsort[i] << "B" << endl;
    for (int i = max_pos + 1; i < n; ++i)
        cout << vsort[i] << "T" << endl;

    return;
}

int main()
{
#ifdef LEE_TEST
    freopen("input19.txt", "r", stdin);
    freopen("output19.txt", "w", stdout);
#endif

    vector<int> vorg;
    vector<int> vsort;
    string tmp;
    while (cin >> tmp)
    {
        if (tmp != ";")
        {
            int num = atoi(tmp.c_str());
            vorg.push_back(num);
            vsort.push_back(num);
            tmp.clear();
        }
        else
        {
            cardTrick(vorg, vsort);
            cout << ";" << endl;      // 用分号分割

            vorg.clear();    // 还原
            vsort.clear();
        }
    }
    cardTrick(vorg, vsort);

    return 0;
}
