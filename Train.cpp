/*
 Train
 
 You work at a train station that��s responsible for rearranging the carriages
 of long vehicles on-the-fly. This is a very complicated work, as carriages 
 have to be attached or removed one-by-one using a single fork.  
 Your job is to calculate the least number of operations you have to perform 
 to have the train rearranged.
  
 Input format 
 You are provided two strings. The first one lists the carriages of the incoming 
 train from head to tail. The second shows how they should look like after leaving 
 the station. Each cargo type is represented by a lowercase letter of the English
 alphabet. You can assume that you have unlimited amount of carriages to attach 
 and unlimited space to store the removed ones. Incoming and outgoing trains never
 consist of more than 34000 carriages, but they are allowed to grow longer during 
 the rearrangement process.
 
 Output format 
 You need to specify a single integer that is equal to the least number of attach 
 and remove operations required to rearrange the train.
 
 Sample input 
 abacdezx
 bascdet
 
 Sample output
 5 
 */
/***
 * ��̱༭���� - ��̬�滮
 * ����d[i, j] ��ʾword1��ǰi���ַ���word2��ǰj���ַ�����С�༭����
 * ��i�ַ�����3��;����Ϊj����ɾ�ģ�������ֻ����ɾ���ֲ���
 *           / d[i-1, j-1], word1[i] == word2[j]
 * d[i, j] = - d[i-1, j] + 1, delete, word1ɾ��i�ַ�
 *           \ d[i, j-1] + 1, insert, word1����j�ַ�
 ***/

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
using namespace std;

#define LEE_TEST

int Train(string word1, string word2) {
    int n1 = word1.size();
    int n2 = word2.size();

    // ֮ǰ��¼�����е�״̬���ڴ����Ľϴ�ͨ����
    // ����ֻ��¼ǰһ�кͺ�һ������
    //vector<vector <int> > vvd(n1+1, vector<int>(n2+1, 0));
    vector<vector <int> > vvd(2, vector<int>(n2+1, 0));

    // init
    for (int j = 0; j <= n2; ++j)  // ��ʼ����һ��
        vvd[0][j] = j;

    for (int i = 1; i <= n1; ++i)
    {
        int curline = i % 2;    // ��¼��ǰ�У���0���ѳ�ʼ�����ӵ�1�п�ʼ
        for (int j = 1; j <= n2; ++j)
        {
            if (j == 1)  // ע��߽�ֵ!!!
            {
                vvd[curline][0] = i;        // ����
                vvd[1-curline][0] = i - 1;  // ��һ��
            }
            if (word1[i-1] == word2[j-1])  // ע���±꣡����
                //vvd[i][j] = vvd[i-1][j-1];
                vvd[curline][j] = vvd[1-curline][j-1];  // 1-curline ��ʾ��һ��
            else
                //vvd[i][j] = std::min(vvd[i-1][j], vvd[i][j-1]);
                vvd[curline][j] = std::min(vvd[1-curline][j], vvd[curline][j-1]) + 1;
        }
    }
    return vvd[n1%2][n2]; // ���һ����n1 % 2
}

int main()
{
#ifdef LEE_TEST
	freopen("input20.txt", "r", stdin);
	freopen("output20.txt", "w", stdout);
#endif
	string s1;
	string s2;

	while(cin >> s1)
	{
		if(s1 != ";")
		{
			cin >> s2;
			cout << Train(s1, s2) << endl;
		}
		else
			cout << ";" << endl;
	}

	return 0;
}
