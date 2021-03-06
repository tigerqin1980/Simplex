/*
	Author：Sun Jiaxuan
	School：Huazhong University of Science and Technology
	Compiler:TDM-GCC 4.7.1 64-bit Release
	Time:2017-09-25 16:30
*/
	
#include <cstdio>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include  <vector>
#include  <fstream>
#include <set>
using namespace std;
vector<vector<double> > Matrix;//储存约束方程的系数矩阵 
double Z;
set<int> P;//迭代器 
size_t cn, bn;

bool Pivot(pair<size_t, size_t> &p)//返回0表示所有的非轴元素都小于0
{
    int x = 0, y = 0;
    double cmax = -INT_MAX;
    vector<double> C = Matrix[0];
    vector<double> B;
    for( size_t i = 0 ; i < bn ; i++ )
    {
        B.push_back(Matrix[i][cn-1]);
    }
    
    for( size_t i = 0 ; i < C.size(); i++ )//在非轴元素中找最大的c
    {
        if( cmax < C[i] && P.find(i) == P.end())
        {
            cmax = C[i];
            y = i;
        }
    }
    if( cmax < 0 )
    {
        return 0;
    }

    double bmin = INT_MAX;//根据θ规则，求最小检验数，确定转出变量 
    for( size_t i = 1 ; i < bn ; i++ )
    {
        double tmp = B[i]/Matrix[i][y];
       if( Matrix[i][y] != 0 && bmin > tmp )
       {
           bmin = tmp;
           x = i;
       }
    }

    p = make_pair(x, y);//存储转入转出变量 

    for( set<int>::iterator it = P.begin() ; it != P.end() ; it++) 
    {
        if( Matrix[x][*it] != 0 )
        {
            P.erase(*it);
            break;
        }
    }
    P.insert(y);
    return true;
}

void pnt()//输出原始系数矩阵 
{
    for( size_t i = 0 ; i < Matrix.size() ; i++ )
    {
        for( size_t j = 0 ; j < Matrix[0].size() ; j++ )
        {
            cout<<Matrix[i][j]<<"\t";
        }
    cout<<endl;
    }
    cout<<"result z:"<<-Matrix[0][cn-1]<<endl;
}

void Gaussian(pair<size_t, size_t> p)//高斯行变换
{
    size_t  x = p.first;
    size_t y = p.second;
    double norm = Matrix[x][y];
    for( size_t i = 0 ; i < cn ; i++ )//主行归一化
    {
        Matrix[x][i] /= norm;
    }
    for( size_t i = 0 ; i < bn && i != x; i++ )
    {
        if( Matrix[i][y] != 0)
        {
            double tmpnorm = Matrix[i][y];
            for( size_t j = 0 ; j < cn ; j++ )
            {
                Matrix[i][j] = Matrix[i][j] - tmpnorm * Matrix[x][j];
            }
        }
    }
}

void solve()
{
    pair<size_t, size_t> t;
    while(1)
    {

        pnt();
        if( Pivot(t) == 0 )//已得到最优解
        {
            return;
        }        
        cout<<t.first<<" "<<t.second<<endl;
        for( set<int>::iterator it = P.begin(); it != P.end()  ; it++ )
        {
            cout<<*it<<" ";
        }//输出迭代后的矩阵
        cout<<endl;
        Gaussian(t);
    }
}

int main()
{
    ifstream fin;
    fin.open("sample.txt");//文件读入。在代码同一目录下创建sample的记事本文件，格式见后面批注
    fin>>cn>>bn;
    for( size_t i = 0 ; i < bn ; i++ )
    { 
        vector<double> vectmp;
        for( size_t j = 0 ; j < cn ; j++)
        {
            double tmp = 0;
            fin>>tmp;
            vectmp.push_back(tmp);
        }
        Matrix.push_back(vectmp);//初始矩阵存储
    }

    for( size_t i = 0 ; i < bn-1 ; i++ )
    {
        P.insert(cn-i-2);
    }
    solve();
}
/////////////////////////////////////
//input:
///* Variables */
//var x1 >= 0;
//var x2 >= 0;
//var x3 >= 0;
///* Object function */
//maximize z: x1 + 14*x2 + 6*x3;
///* Constrains */
//s.t. con1: x1 + x2 + x3 <= 4;
//s.t. con2: x1  <= 2;
//s.t. con3: x3  <= 3;
//s.t. con4: 3*x2 + x3  <= 6;
//end;
/////////////////////////////////////
//sample.txt文件示例:
//8 5
//1 14 6 0 0 0 0 0
//1 1 1 1 0 0 0 4
//1 0 0 0 1 0 0 2
//0 0 1 0 0 1 0 3
//0 3 1 0 0 0 1 6
/////////////////////////////////////

