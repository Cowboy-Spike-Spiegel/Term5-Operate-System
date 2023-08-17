#include "rf.h"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>


using namespace std;

int shared_data; //共享数据
int read_count = 0; //读者数量
sem_t rp_wrt; //互斥变量，用于控制对缓冲区的访问，初始化为 1
sem_t mutex; //互斥变量，用于控制 read_count 的互斥访问，初始化为 1


void initing()
{
	sem_init(&rp_wrt, 0, 1);
	sem_init(&mutex, 0, 1);
}


void* writer(void* param)
{
	int* id = (int*)param;
	sem_wait(&rp_wrt); //等待访问权限
	output_str += "-----Writer visit critical section.\n";
	output_str += "This is Writer, with id="+to_string(*id)+".\n";
	shared_data = *id;
	sleep(10);
	output_str += "-----Writer left critical section.\n";
	sem_post(&rp_wrt); //释放访问权限
}

void* reader(void* param) 
{
	int* id = (int*)param;
	sem_wait(&mutex); //互斥访问 read_count
	read_count++;
	if (read_count == 1) //如果是第一个读进程，申请获取访问权限
	{
		sem_wait(&rp_wrt);
		output_str += "-----Reader(the fitst) visit critical section.\n";
	}
		
	sem_post(&mutex);
	output_str += "This is Reader, with id="+to_string(*id)+", shared_data="+to_string(shared_data)+".\n";
	sleep(10);
	sem_wait(&mutex);
	read_count--;
	
	if (read_count == 0) //如果是最后一个读进程，释放访问权限
	{
		output_str += "-----Reader(the last) left critical section.\n";
		sem_post(&rp_wrt);
	}
	sem_post(&mutex);
}

