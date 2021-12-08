#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/sem.h>

#define MEM_NAME "first.c"

struct sembuf sem_lock = {0,-1,1}, sem_open = {0,1,0};

typedef struct data_struct{
	time_t firstTime;
	pid_t firstPid;
}data_struct;

int main(int argc, char** argv){

	sleep(1);
	
	key_t key = ftok(MEM_NAME, 1);
	int shmem_id = shmget(key, sizeof(data_struct), 0666);

	if(shmem_id < 0){
		perror("shmget error");
		exit(-1);
	}

	void* shmVal = shmat(shmem_id, NULL, 0);

	if(shmVal == (void*)-1){
		perror("shmat error");
		exit(-2);
	}
	
	int sem_id = semget(key, 1, IPC_CREAT | 0666);
	if(sem_id < 0){
		perror("sem error");
		exit(-3);
	}
	semop(sem_id, &sem_lock, 1);
	//sleep(2);	
	time_t curTime = time(NULL);
	data_struct val = *((data_struct*)shmVal);
	printf("Curent time: %s", ctime(&curTime));
	printf("Received val :\n Time:%s Pid:%d\n", ctime(&val.firstTime), val.firstPid);
	
	semop(sem_id, &sem_open, 1);
	shmdt(shmVal);

	return 0;

}
