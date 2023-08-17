#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int isEven(int n);   //Determine if the number is even
int main(int argc ,char* argv[])
{
    int n =atoi(argv[1]);
	if(n<=0) {
		printf("The input number is not a positive integer\n");
		return 0;
	}

	pid_t pid=fork();
	if(pid==-1) {
		printf("Child process create failed\n");
		return 0;
	}
	//child process
	else if(pid == 0) {
		printf("%d ",n);
			while(n!=1) {
				if (isEven(n)==1)
					n/=2;
				else
					n=3*n+1;
				printf("%d ",n);
			}
			printf("\n");
	}
	//parent process
	else if(pid >0) {
			wait(NULL);
			exit(0);
	}
	return 0;
}
int isEven(int n)
{
	if(n&1)
		return 0;
	else
		return 1;
}