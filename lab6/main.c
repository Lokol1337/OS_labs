#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int num = 0;
void* funW(void* data){
	while(1){
		pthread_mutex_lock(&mutex);
		num = num + 1;
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void* funR(void* data){
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		printf("thread id: %lu\nnum status: %d\n",pthread_self(), num);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
	
}

int main(int argc, char** argv){

		
	pthread_t w,r[10];
	

	for(int i = 0; i < 10; i++){
		pthread_create(&r[i], NULL, funR, NULL);
	}

	pthread_create(&w, NULL, funW, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	pthread_join(w,NULL);	
	
	return 0;

}
