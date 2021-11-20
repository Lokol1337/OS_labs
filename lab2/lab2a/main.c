#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

typedef struct data_struct{
	time_t parentTime;
	pid_t parentPid;
}data_struct;


int main(int argc, char** argv){
	int pipeArray[2];
	pid_t pid;
	pipe(pipeArray);
		
	switch(pid = fork()){
		case -1:
			printf("ERROR fork()\n");
			return -1;
		case 0:
			sleep(5);
 			printf("[CHILD]\n");
			data_struct pack1;
			close(pipeArray[1]);
			read(pipeArray[0], &pack1, sizeof(data_struct));
			close(pipeArray[0]);
			//sleep(5);
			time_t curTime = time(NULL);
			printf("[CHILD] Time: %s", ctime(&curTime));
			printf("[CHILD] Package received from parent:\n - %s - %d\n", ctime(&pack1.parentTime), pack1.parentPid);
			return 0;
		default:
			printf("[PARENT]\n");
			time_t	pTime = time(NULL);
			data_struct pack2 = {pTime, getpid()};
			close(pipeArray[0]);
			write(pipeArray[1], (void*)(&pack2), sizeof(data_struct));
			close(pipeArray[1]);
			printf("[PARENT] Parent(%d) send some data\n", pack2.parentPid);
			wait(0);
			return 0;

	}





	return 0;
}
