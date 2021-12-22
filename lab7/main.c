#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int num = 0;
void* funW(void* data){
	while(1){
		pthread_rwlock_wrlock(&rwlock);
		num = num + 1;	
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
	pthread_exit(NULL);
}

void* funR(void* data){
	while(1){
		pthread_rwlock_rdlock(&rwlock);
		printf("thread id: %lu\nnum status: %d\n",pthread_self(), num);
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
	pthread_exit(NULL);
	
}

int main(int argc, char** argv){

		
	pthread_t w,r[10];
	
	pthread_create(&w, NULL, funW, NULL);

	for(int i = 0; i < 10; i++){
		pthread_create(&r[i], NULL, funR, NULL);
	}
	

	pthread_join(w,NULL);		
	for(int i = 0; i < 10;i++){
		pthread_join(r[i],NULL);
	}
	pthread_rwlock_destroy(&rwlock);

	return 0;

}
