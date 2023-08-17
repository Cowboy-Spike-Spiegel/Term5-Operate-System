#include "wf.h"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

using namespace std;

int shared_data; //共享数据
int read_count; //读者数量
int write_count; //写者数量
sem_t rp_wrt; //互斥变量,控制对缓冲区的访问，初始化为 1
sem_t cs_read; //互斥变量，表示读者排队信号，初始化为 1
sem_t mutex_w; //互斥变量，控制 write_count 的互斥访问，初始化为 1
sem_t mutex_r; //互斥变量，控制 read_count 的互斥访问，初始化为 1
sem_t wp_wrt; //互斥变量，控制对缓冲区的访问，初始化为1

string output_file = "output2.txt";

void initing()
{
	sem_init(&rp_wrt, 0, 1);
	sem_init(&wp_wrt, 0, 1);
	sem_init(&cs_read, 0, 1);
	sem_init(&mutex_w, 0, 1);
	sem_init(&mutex_r, 0, 1);
}


void* writer(void* param)
{
	int* id = (int*)param;

	sem_wait(&mutex_w); //互斥访问 write_count
	write_count++;
	if (write_count == 1) //如果是第一个写进程，申请获取读进程排队权限
	{
		sem_wait(&cs_read);
		output_str += "-----Writer(the fitst) visit critical section.\n";
	}
	sem_post(&mutex_w);
	sem_wait(&wp_wrt); //申请缓冲区访问权限
	
	output_str += "This is Writer, with id="+to_string(*id)+".\n";
	shared_data = *id;
	sleep(10);
	
	sem_post(&wp_wrt);
	sem_wait(&mutex_w);
	write_count--;
	if (write_count == 0)
	{
		output_str += "-----Writer(the last) left critical section.\n";
		sem_post(&cs_read); //如果是最后一个写进程，释放读进程排队权限，允许其排队访问
	}
	sem_post(&mutex_w);
}

void* reader(void* param)
{
	int* id = (int*)param;

	sem_wait(&cs_read); //申请排队权限
	sem_wait(&mutex_r); //互斥访问 read_count
	read_count++;
	if (read_count == 1) //如果是第一个读者，申请访问权限
	{
		sem_wait(&wp_wrt);
		output_str += "-----Reader visit critical section.\n";
	}
	sem_post(&mutex_r);
	sem_post(&cs_read); //释放排队权限

	output_str += "This is Reader, with id="+to_string(*id)+", shared_data="+to_string(shared_data)+".\n";
	sleep(10);

	sem_wait(&mutex_r);
	read_count--;
	if (read_count == 0) //如果是最后一个读者，释放缓冲区访问权限
	{
		output_str += "-----Reader left critical section.\n";
		sem_post(&wp_wrt);
	}
	sem_post(&mutex_r);
}
