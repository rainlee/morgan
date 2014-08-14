/*
 Free fall
 
 Flombi is the young hero of a computer game. Whilst completing his quest, he is
 jumping across horizontal platforms to find clues to the mystery he is bound to solve.
 
 His task is not easy as his enemies could attack him any time! Flombi managed to find 
 a way to protect himself while he is standing on a platform, but is still vulnerable 
 when falling. Thus, your goal is to find a path for him to get to the ground 
 (Y coordinate of 0) from his starting point with as little time spent in the 
 air as possible.
 
 At the beginning of the game, Flombi is released at the 0,H coordinates with horizontal 
 and vertical speeds of 0, and will fall vertically until he reaches the first platform.
 His first goal is to get to the ground in safety. Whenever he wants to leave a platform,
 he can jump from either the left or the right edge of it. When doing so, his starting 
 vertical speed will always be 0, and he can choose any horizontal speed between 0.001 
 and MAXV. His direction depends on the edge he chooses – he can only jump to the left 
 from the left edge, and vice versa. As soon as Flombi hits a platform or the ground, 
 he loses all his speed immediately.
 
 You can assume that Flombi is shapeless and dimensionless; the height of the platforms 
 is zero. There’s no wind or atmosphere, therefore the horizontal speed will not change 
 during the fall. The vertical speed will be increased by G=9.8 units/s2, so you can assume 
 that the fall from the height of D units will take (2D/G)1/2 seconds.
 
 Input format 
 The first two lines contain one positive integer each, describing the value of H and MAXV 
 respectively. The following lines contain three integers each, separated by whitespaces: 
 Xi Yi Wi, denoting the X coordinate of the left edge of platform #i, it’s Y coordinate,
 and it’s width respectively. Platforms are indexed from 0, and the 0th platform will 
 always be the one that Flombi hits first when he starts his fall at the beginning of the game. 
 You can assume that Y0 < H and MAXV>0.001.
 
 Output format 
 You need to specify the path you suggest to Flombi, one step per line. Each line must contain 
 three values separated by whitespaces: D T V – where D is an upper case letter defining the 
 direction Flombi should take (L for left, R for right), T is the index of the target platform 
 that Flombi hits when making this move, and V is the unsigned value of the horizontal starting 
 speed. The speed will be treated as a double (64-bit IEEE 754) value; digits that can’t fit into 
 this precision are rounded during evaluation.
 The index of the ground is -1, thus the last line in your output must contain the T value of -1.
 
 Sample input 
 15
 1
 -1 14 2
 0 13 3
 -3 11 4
 0 9 6
 -5 6 5
 -6 5 2
 -1 5 9
 6 4 3
 -3 3 8
 -8 1 5
 
 Sample output 
 L 2 0.5
 L 4 0.5
 R 6 0.5
 R 7 0.5
 L -1 0.5
 */
/***
 * 题意：人的初始位置在(0, H), 下面有n块跳板，要用最快的速度落到地面(y=0)
 * 水平为匀速直线运动v([0.001, MAXV]), 竖直方向自由落体(g=9.8)
 * 可以往左右两个方向跳
 * 使用动态规划，t[i]表示到达第i块跳板的最短时间
 * 则，t[i] = min{t[0], t[1], ... t[i-1]}，前i块板子能到达i的最短时间
 * 假设现在人在第j块板，求出他可以跳往的所有板子i的时间
 * 则，t[i] = min(t[i], t[j] + cost)
 ***/
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <climits>
#include <cstdlib>
using namespace std;

#define LEE_TEST

struct path
{
    char d;   // direction
    int t;    // target platform
    double v;    // speed
};

struct platform
{
    int id;      // 平台id

    int xl;
    int y;
    int xr;

    double t;    // 到达该平台的最短时间
    path p;      // t对应的path
    int prep;    // 前一个platform

    platform(int id): id(id), xl(0), y(0), xr(0), t(LONG_MAX), prep(-2) {}  // -1 是 ground
};

void freeFall(vector<platform> &vp, double maxv)
{
    if (vp.size() == 0)
        return;

    platform ptmp(-1);     // 将ground添加到最后
    ptmp.y = 0;
    ptmp.xl = INT_MIN;
    ptmp.xr = INT_MAX;
    vp.push_back(ptmp);

    vp[0].t = 0;
    for (int i = 0; i < vp.size(); ++i)  // 求出每个平台向左和向右能到的最远距离,从i一步跳到j
    {
        for (int j = i+1; j < vp.size(); ++j)  // left
        {
            int dh = vp[i].y - vp[j].y;   // 竖直距离
            double t = sqrt(2 * dh / 9.8);

            if (j == vp.size()-1)  // 到达ground
            {
                double t2 = vp[i].t + t;
                if (t2 < vp[j].t)
                {
                    vp[j].t = t2;
                    vp[j].p.d = 'L';
                    vp[j].p.t = vp[j].id;
                    vp[j].p.v = maxv;      // 只要能达到的速度都行，这里取最大
                    vp[j].prep = i;
                    break;
                }
            }

            if (vp[j].xl >= vp[i].xl)  // j在i右边，越过j
                continue;

            int dr = vp[i].xl - vp[j].xr; // i左端点距j右端点距离
            int dl = vp[i].xl - vp[j].xl; // i左端点距j左端点距离
            if ((dl > 0) && (maxv * t < dr))   // j在i左边 && 最大速度都无法到达
                continue;

            // 可以从i到达j
            double t2 = vp[i].t + t;
            if (t2 < vp[j].t)
            {
                vp[j].t = t2;
                vp[j].p.d = 'L';
                vp[j].p.t = vp[j].id;
                vp[j].p.v = min(maxv, dl/t);  // 尽量最远
                vp[j].prep = i;
            }

            if (maxv * t <= dl)  // 不能超过j了，无法到达下一层
                break;
        } // for

        for (int j = i+1; j < vp.size(); ++j)  // right
        {
            int dh = vp[i].y - vp[j].y;   // 竖直距离
            double t = sqrt(2 * dh / 9.8);

            if (j == vp.size()-1)  // 到达ground
            {
                double t2 = vp[i].t + t;
                if (t2 < vp[j].t)
                {
                    vp[j].t = t2;
                    vp[j].p.d = 'R';
                    vp[j].p.t = vp[j].id;
                    vp[j].p.v = maxv;      // 只要能达到的速度都行，这里取最大
                    vp[j].prep = i;
                    break;
                }
            }

            if (vp[j].xr <= vp[i].xr)  // j在i左边，越过j
                continue;

            int dl = vp[j].xl - vp[i].xr; // i右端点距j左端点距离
            int dr = vp[j].xr - vp[i].xr; // i右端点距j右端点距离
            if ((dr > 0) && (maxv * t < dl))   // j在i右边 && 最大速度都无法到达
                continue;

            // 可以从i到达j
            double t2 = vp[i].t + t;
            if (t2 < vp[j].t)
            {
                vp[j].t = t2;
                vp[j].p.d = 'R';
                vp[j].p.t = vp[j].id;
                vp[j].p.v = min(maxv, dr/t);  // 尽量最远
                vp[j].prep = i;
            }

            if (maxv * t <= dr)  // 不能超过j了，无法到达下一层
                break;
        } // for
    } // for

    vector<int> vpath;           // vp中记录路径的prep，这里逆向还原该路径
    vpath.push_back(vp.size()-1);   // 最后一个是ground
    int prep = vp[vp.size()-1].prep;
    //while (prep != -2)
    while (prep != 0)    // 回溯到第0个
    {
        vpath.push_back(prep);
        prep = vp[prep].prep;
    }

    // output
    for (int i = vpath.size()-1; i >= 0; --i)
        cout << vp[vpath[i]].p.d << " " << vp[vpath[i]].p.t << " " << vp[vpath[i]].p.v << endl;

    return;
}

int main()
{
#ifdef LEE_TEST
    freopen("input22.txt", "r", stdin);
    freopen("output22.txt", "w", stdout);
#endif // LEE_TEST

    int h = 0;
    double maxv = 0;
    vector<platform> vp;
    while (cin >> h >> maxv)
    {
        string sch;
        int pid = 0;
        while (cin >> sch)
        {
            if (sch != ";")
            {
                platform ptmp(pid);     // 记录平台号
                ptmp.xl = atoi(sch.c_str());
                cin >> ptmp.y >> ptmp.xr;
                if (ptmp.y > h)
                    continue;     // h以上的平台不可能到达，不用存
                ptmp.xr += ptmp.xl;       // 输入的是宽度
                vp.push_back(ptmp);
                pid++;
            }
            else  // 本组数据读完了
            {
                freeFall(vp, maxv);
                vp.clear();
                cout << ";" << endl;
                break;
            }
        }
    }
    freeFall(vp, maxv);  // 处理最后一组

    return 0;
}
