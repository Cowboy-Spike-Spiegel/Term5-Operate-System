#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

// global variables
int maximum=0;
int minimum=0;
float average=0;
int numbers=0;

void* cal_maximum(void*paragram);
void* cal_minimun(void*paragram);
void* cal_average(void*paragram);


int main(void)
{
	int datas[1024];

	// start calculate
	char c=0;
	while(c!='\n') {
		scanf("%d%c",&datas[numbers],&c);
		numbers++;
	}

	// start thread
	pthread_t pid1;
	pthread_t pid2;
	pthread_t pid3;
	pthread_create(&pid1, NULL, cal_maximum, datas);
	pthread_create(&pid2, NULL, cal_minimun, datas);
	pthread_create(&pid3, NULL, cal_average, datas);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	pthread_join(pid3, NULL);

	// output
	printf("minimum=%d\nmaximum=%d\naverage=%f\n", average, minimum, maximum);
}

void* cal_maximum(void* paragram)
{
	int *datas=(int*)paragram;
	maximum=datas[0];
	for(int i=0;i<numbers;i++)
		if(datas[i]>maximum)
			maximum=datas[i];

	pthread_exit(0);
}

void* cal_minimun(void* paragram)
{
	int *datas=(int*)paragram;
	minimum=datas[0];
	for(int i=0;i<numbers;i++)
		if(datas[i]<minimum)
			minimum=datas[i];

	pthread_exit(0);
}

void *cal_average(void* paragram)
{
	int *datas=(int*)paragram;
	for(int i=0;i<numbers;i++)
		average+=datas[i];
	average/=numbers;

	pthread_exit(0);
}
