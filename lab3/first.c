#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>


#define MEM_NAME "first.c"

typedef struct data_struct{
	time_t firstTime;
	pid_t firstPid;
}data_struct;

int main(int argc, char** argv){

	struct shmid_ds shmidStat;

	key_t key = ftok(MEM_NAME, 1);
	int shmem_id = shmget(key, sizeof(data_struct), IPC_CREAT | 0666);

	if(shmem_id < 0){
		perror("shmget error");
		exit(-1);
	}

	void* shmVal = shmat(shmem_id, NULL, 0);
	
	
        shmctl(shmem_id, IPC_STAT,&shmidStat);

        if(shmidStat.shm_nattch > 1){
                printf("shmget error : File already exists.\n");
                exit(-3);
        }
	

	if(shmVal == (void*)-1){
		perror("shmat error");
		exit(-2);
	}

	int a = 1;
	while(a == 1){
		time_t valTime = time(NULL);
		data_struct val = {valTime, getpid()};

	        *((data_struct*)shmVal) = val;
        	printf("Placed val :\n Time:%s Pid:%d\n",ctime(&val.firstTime), val.firstPid);
        	sleep(5);
	
	}

	shmdt(shmVal);
	shmctl(shmem_id, IPC_RMID, NULL);
	
	return 0;

}
