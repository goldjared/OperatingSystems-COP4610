#include <pthread.h>
#include <stdio.h>

const int CAP = 2000000;
pthread_mutex_t lock;

void *deposit(void *arg) {
	int *balance = (int *)arg;
	int bonusCounter = 0;
	int transactions = 0;
	while(transactions < CAP) {
		// entry
		pthread_mutex_lock(&lock);
		// critical
		(*balance)++;
		transactions++;
		if(*balance % 20 == 0) {
			(*balance) += 50;
			transactions += 50;
			bonusCounter++;
			// for the case of a bonus going over CAP, we adjust for the diff
			while(transactions > CAP) {
				(*balance)--;
				transactions--;
				bonusCounter--;
			}
		}

		// exit
		pthread_mutex_unlock(&lock);
		// remainder (iterate)
	}

	printf("THREAD A: %d deposit operations performed. Bonus recieved %d times. Balance: %d\n", transactions, bonusCounter, *balance);

	return NULL;
}

void *withdraw(void *arg)	{
	int *balance = (int *)arg;
	int transactions = 0;
	
	for(int t = 0; t < CAP; t++) {
		// entry
		pthread_mutex_lock(&lock);
		// critical
		(*balance)--;
		transactions++;
		// exit
		pthread_mutex_unlock(&lock);
		// remainder (iterate)
	}

	printf("THREAD B: %d withdraw operations performed. Balance: %d\n", transactions, *balance);
	return NULL;
}

int main() {
	int balance = 0;
	pthread_t t1;
	pthread_t t2;
	// init mutex lock
	pthread_mutex_init(&lock, NULL);

	pthread_create(&t1, NULL, deposit, &balance);
	pthread_create(&t2, NULL, withdraw, &balance);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("PARENT: Final Balance: %d\n", balance);
	pthread_mutex_destroy(&lock);
	
	return 0;
}
