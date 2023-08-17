#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <pthread.h>
#include <cstdlib>
//#include "rf.h"
#include "wf.h"

using namespace std;

typedef struct order
{ 
    char rw;			//读写进程标志
    int spendtime;		//读写时间
}order;

vector<order>orders;		//命令集合
string fileName = "data.txt";   //文件名称
int orderNum = 20;     		//命令数量

string output_name = "output.txt";
ofstream outFile;

string output_str = "";

int main()
{
    // solve input
    ifstream file(fileName);
    order t;
    for (int i = 0; i < orderNum; i++) { 
        file >> t.rw >> t.spendtime; 
        orders.push_back(t); 
    }
    
    // solve output
    outFile.open(output_name);

    pthread_t * p = (pthread_t*)malloc(orderNum * sizeof(pthread_t)); 
    int* p_id = (int*)malloc(orderNum * sizeof(int));//读写进程创建

    initing();  //互斥量初始化
    for(int i=0;i<orderNum;i++)
    {
        if(orders[i].rw=='R')//创建读进程
        {
            output_str += to_string(i+1) + " process for Reader created.\n";
            p_id[i]=i+1;
            pthread_create(&p[i],NULL,reader,&p_id[i]);
           
        }
        else//创建写进程
        {
            output_str += to_string(i+1) + " process for Writer created.\n";
            p_id[i]=i+1;
            pthread_create(&p[i], NULL, writer, &p_id[i]);
        }
    }
    
    //等待线程全部结束后主线程结束
    for(int i=0;i<orderNum;i++)
        pthread_join(p[i],NULL);
        
    // output
    cout << output_str;
    outFile << output_str;
    outFile.close();

    return 0;
}
