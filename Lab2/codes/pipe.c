#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/wait.h>
void changeCase(char* buff);
int main()
{
	int fd1[2],fd2[2];
	int pipe1=pipe(fd1);
	int pipe2=pipe(fd2);
	char buff[2049]={0};
	if (pipe1<0||pipe2<0) {
		printf("pipe create failed\n");
		return 0 ;
	}
	
	// create fork
	pid_t pid=fork();
	if(pid==-1) {
		printf("Child process create failed\n");
		return 0 ;
	}
	//child process
	else if(pid == 0) {
		close(fd1[0]);   //Turn off pipe1 read
		close(fd2[1]);	//Turn off pipe1 read
		gets(buff);
		printf("child process send message: %s\n",buff);
		write(fd1[1],buff,strlen(buff));
		memset(buff,0,strlen(buff));
		read(fd2[0],buff,2048);
		printf("child process receive message: %s\n",buff);
	}
	//parent process
	else if(pid >0) {
		close(fd1[1]);//Turn off pipe1 write
		close(fd2[0]);//Turn off pipe2 read
		read(fd1[0],buff,2048);
		printf("parent process receive message: %s\n",buff);
		changeCase(buff);
		printf("parent process send message: %s\n",buff);
		write(fd2[1],buff,strlen(buff));
		wait(NULL);
		exit(0);
	}
	return 0;
}


void changeCase(char* buff)
{
	for(int i=0;i<strlen(buff);i++) {
        if(buff[i]>='a'&&buff[i]<='z') 
            buff[i]-=32;
        else if(buff[i]>='A'&&buff[i]<='Z') 
            buff[i]+=32;
	}
}
