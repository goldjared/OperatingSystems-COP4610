#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void *thread1work(void *arg) {
        // entry
        pthread_mutex_lock(&lock1);
        sleep(1);
        pthread_mutex_lock(&lock2);
        // critical
        printf("thread1 critical. \n");
        // exit
        pthread_mutex_unlock(&lock2);
        pthread_mutex_unlock(&lock1);

	return NULL;
}

void *thread2work(void *arg) {
        // entry
        pthread_mutex_lock(&lock2);
        sleep(1);
        pthread_mutex_lock(&lock1);
        // critical
        printf("thread2 critical. \n");
        // exit
        pthread_mutex_unlock(&lock1);
        pthread_mutex_unlock(&lock2);

	return NULL;
}

int main() {
	pthread_t t1;
	pthread_t t2;
	// init mutex lock
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);

	pthread_create(&t1, NULL, thread1work, NULL);
	pthread_create(&t2, NULL, thread2work, NULL);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
	
	return 0;
}
