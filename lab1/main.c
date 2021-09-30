#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void function(){
	printf("atexit отработал --PID:%d -- PPID:%d\n",getpid(), getppid());
}

int main(int argc, char** argv){
	pid_t pid;
	pid = fork();
	int status;
	atexit(function);
	switch(pid){
		case 0: printf("Ребёнок: %d\nМой родитель:%d\n",getpid(), getppid()); break;
		case -1: printf("Я ошибка\n"); break;
		default: 
			printf("Родитель: %d\nМой родитель:%d\n", getpid(),getppid()); 	
			wait(&status); 
			printf("Cтатус ребёнка: %d\n",WEXITSTATUS(status)); 
			break;
	}
	
	return 0;

}
