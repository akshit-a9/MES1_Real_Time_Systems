#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


int count=0;

pthread_mutex_t lock;

void* readTemp(void *delay){
	int i=0;
     for(i=0;i<100000;i++){
     	printf("Temperature is %d\n",count);
     	pthread_mutex_lock(&lock);
     	count++;
     	pthread_mutex_unlock(&lock);
     	sleep(*(int *)delay);
     	}
     	return NULL;
}

void* readPressure(void *delay){
	int i=0;
     for(i=0;i<100000;i++){
     	printf("Pressure is %d\n",count);
     	pthread_mutex_lock(&lock);
     	count++;
     	pthread_mutex_unlock(&lock);
     	sleep(*(int *)delay);
     	}
}


int main(){
	int delay;
	delay = 1;
	pthread_t thread_id1,thread_id2;
	printf("Hello from main");
	pthread_create(&thread_id1,NULL,readTemp,(void *)&delay);
	pthread_create(&thread_id2,NULL,readPressure,(void *)&delay);
	pthread_join(thread_id1,NULL);
	pthread_join(thread_id2,NULL);
	printf("Final count is %d\n",count);
	return 0;
	}
