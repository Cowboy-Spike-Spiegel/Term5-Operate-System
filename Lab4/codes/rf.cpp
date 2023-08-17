#include "rf.h"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>


using namespace std;

int shared_data; //��������
int read_count = 0; //��������
sem_t rp_wrt; //������������ڿ��ƶԻ������ķ��ʣ���ʼ��Ϊ 1
sem_t mutex; //������������ڿ��� read_count �Ļ�����ʣ���ʼ��Ϊ 1


void initing()
{
	sem_init(&rp_wrt, 0, 1);
	sem_init(&mutex, 0, 1);
}


void* writer(void* param)
{
	int* id = (int*)param;
	sem_wait(&rp_wrt); //�ȴ�����Ȩ��
	output_str += "-----Writer visit critical section.\n";
	output_str += "This is Writer, with id="+to_string(*id)+".\n";
	shared_data = *id;
	sleep(10);
	output_str += "-----Writer left critical section.\n";
	sem_post(&rp_wrt); //�ͷŷ���Ȩ��
}

void* reader(void* param) 
{
	int* id = (int*)param;
	sem_wait(&mutex); //������� read_count
	read_count++;
	if (read_count == 1) //����ǵ�һ�������̣������ȡ����Ȩ��
	{
		sem_wait(&rp_wrt);
		output_str += "-----Reader(the fitst) visit critical section.\n";
	}
		
	sem_post(&mutex);
	output_str += "This is Reader, with id="+to_string(*id)+", shared_data="+to_string(shared_data)+".\n";
	sleep(10);
	sem_wait(&mutex);
	read_count--;
	
	if (read_count == 0) //��������һ�������̣��ͷŷ���Ȩ��
	{
		output_str += "-----Reader(the last) left critical section.\n";
		sem_post(&rp_wrt);
	}
	sem_post(&mutex);
}

