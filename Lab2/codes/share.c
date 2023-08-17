#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<string.h>
int isEven(int n); //Determine if the number is even
int main(int argc ,char* argv[])
{
	// get first number
	int n = atoi(argv[1]);
	if(n<=0) {
		printf("The input number is not a positive integer\n");
		return 0;
	}
	
	// create share memory file
	const char* name = "share memory";
	int shm_size = 4096;
	int shm_fd = shm_open(name, O_CREAT|O_RDWR, 0777);  //Create shared memory file
	ftruncate(shm_fd, shm_size); //Reset the file size
	
	// create fork
	pid_t pid=fork();	
	if(pid==-1) {
		printf("Child process create failed\n");
		return 0;
	}
	//child process
	else if(pid == 0) {
		void *shm_fptr=mmap(0,shm_size,PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
		char buff[2048]={'\0'};
		int str_len = sprintf(buff,"%d ",n);
		sprintf(shm_fptr,"%s",buff);
		shm_fptr+=str_len;
		memset(buff,0,str_len);
		while(n!=1) {
			if (isEven(n)==1)
				n/=2;
			else
				n=3*n+1;
			// write into share memory
			str_len=sprintf(buff,"%d ",n);
			sprintf(shm_fptr,"%s",buff);
			shm_fptr+=str_len;
			memset(buff,0,str_len);
		}
	}
	//parent process
	else if(pid >0) {
		char *buff=NULL;
		wait(NULL);
		// read from share memory
		void* shm_fptr=mmap(0,shm_size,PROT_READ,MAP_SHARED,shm_fd,0);
		printf("%s\n",(char*)shm_fptr);
		shm_unlink(name);//delete share memory
		exit(0);
	}
	return 0;
}


int isEven(int n) {
	if(n&1)
		return 0;
	else
		return 1;
}