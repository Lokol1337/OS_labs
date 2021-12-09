#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int num = 0;
void* funW(void* data){
	int i = 0;
	while(1){
		i += 1;
		pthread_mutex_lock(&mutex);
		num = num + 1;
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void* funR(void* data){
	int i = 0;
	while(1){
		i += 1;
		pthread_mutex_lock(&mutex);
		printf("thread id: %lu\nnum status: %d\n",pthread_self(), num);
		pthread_mutex_unlock(&mutex);
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
	
	return 0;

}
