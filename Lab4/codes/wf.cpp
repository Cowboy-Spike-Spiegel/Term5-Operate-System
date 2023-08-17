#include "wf.h"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

using namespace std;

int shared_data; //��������
int read_count; //��������
int write_count; //д������
sem_t rp_wrt; //�������,���ƶԻ������ķ��ʣ���ʼ��Ϊ 1
sem_t cs_read; //�����������ʾ�����Ŷ��źţ���ʼ��Ϊ 1
sem_t mutex_w; //������������� write_count �Ļ�����ʣ���ʼ��Ϊ 1
sem_t mutex_r; //������������� read_count �Ļ�����ʣ���ʼ��Ϊ 1
sem_t wp_wrt; //������������ƶԻ������ķ��ʣ���ʼ��Ϊ1

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

	sem_wait(&mutex_w); //������� write_count
	write_count++;
	if (write_count == 1) //����ǵ�һ��д���̣������ȡ�������Ŷ�Ȩ��
	{
		sem_wait(&cs_read);
		output_str += "-----Writer(the fitst) visit critical section.\n";
	}
	sem_post(&mutex_w);
	sem_wait(&wp_wrt); //���뻺��������Ȩ��
	
	output_str += "This is Writer, with id="+to_string(*id)+".\n";
	shared_data = *id;
	sleep(10);
	
	sem_post(&wp_wrt);
	sem_wait(&mutex_w);
	write_count--;
	if (write_count == 0)
	{
		output_str += "-----Writer(the last) left critical section.\n";
		sem_post(&cs_read); //��������һ��д���̣��ͷŶ������Ŷ�Ȩ�ޣ��������Ŷӷ���
	}
	sem_post(&mutex_w);
}

void* reader(void* param)
{
	int* id = (int*)param;

	sem_wait(&cs_read); //�����Ŷ�Ȩ��
	sem_wait(&mutex_r); //������� read_count
	read_count++;
	if (read_count == 1) //����ǵ�һ�����ߣ��������Ȩ��
	{
		sem_wait(&wp_wrt);
		output_str += "-----Reader visit critical section.\n";
	}
	sem_post(&mutex_r);
	sem_post(&cs_read); //�ͷ��Ŷ�Ȩ��

	output_str += "This is Reader, with id="+to_string(*id)+", shared_data="+to_string(shared_data)+".\n";
	sleep(10);

	sem_wait(&mutex_r);
	read_count--;
	if (read_count == 0) //��������һ�����ߣ��ͷŻ���������Ȩ��
	{
		output_str += "-----Reader left critical section.\n";
		sem_post(&wp_wrt);
	}
	sem_post(&mutex_r);
}
