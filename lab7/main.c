#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static pthread_rwlockattr_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int num = 0;
void* funW(void* data){
	while(1){
		pthread_rwlock_trywrlock(&rwlock);
		num = num + 1;	
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
}

void* funR(void* data){
	while(1){
		pthread_rwlock_tryrdlock(&rwlock);
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
	
	pthread_rwlock_destroy(&rwlock);
	pthread_join(w,NULL);		
	return 0;

}
