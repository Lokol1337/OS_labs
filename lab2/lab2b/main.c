#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>

#define FIFO_NAME "fifo_file"

typedef struct data_struct{
	time_t parentTime;
	pid_t parentPid;
}data_struct;

int main(int argc, char** argv){
	pid_t pid;
	
	switch(pid = fork()){
	case -1:
		printf("Error fork()");
		return -1;
	case 0:
		sleep(1);
		printf("[CHILD]\n");
		int fifoRead = open(FIFO_NAME, O_RDONLY);
		
		if(fifoRead < 0){
			printf("[CHILD] Error fifo read\n");
			return -1;
		}
		sleep(5);
		time_t curTime = time(NULL);
		data_struct pack1;
		read(fifoRead, &pack1, sizeof(data_struct));
		close(fifoRead);
		printf("[CHILD] Time: %s", ctime(&curTime));
		printf("[CHILD] Package received from parent:\n - %s - %d\n", ctime(&pack1.parentTime), pack1.parentPid);
		return 0;

	default:
		printf("[PARENT]\n");
		unlink(FIFO_NAME);
		int fifo = mkfifo(FIFO_NAME, 0777);
		
		if(fifo < 0){
			printf("[PARENT] Error fifo creating\n");
			return -1;
		}
		
		int fifoWrite = open(FIFO_NAME, O_WRONLY);
		
		if(fifoWrite < 0){
			printf("[PARENT] Error fifo write\n");
			return -1;
		}

		time_t pTime = time(NULL);
		pid_t curPid = getpid();
		//printf("[PARENT] Parent(%d) send some data\n", curPid);
		data_struct pack2 = {pTime, curPid};
		write(fifoWrite, &pack2, sizeof(data_struct));
		close(fifoWrite);
		printf("[PARENT] Parent(%d) send some data\n", curPid);
		wait(0);
		return 0;
	}

}
