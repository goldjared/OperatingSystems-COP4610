/*
* WHAT IS HAPPENING:
* -> Race condition.
* The counter is a different value each time. This is because the different threads are accessing the shared value at different points. Simply, the total count should be 10*100000 (10 threads, 100k iterations each). If we secure the shared memory value, the threads should all access the true count value.
*
* *** output at end file ***
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREADS 10
#define INCREMENTS_PER_THREAD 100000

int counter = 0; // Shared counter
pthread_mutex_t counter_mutex; // Mutex declaration 
sem_t counter_sem; // Semaphore declaration 

void* increment_counter(void* threadid) {

    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&counter_mutex); // Lock the mutex before accessing the counter 
        counter++; // Incrementing the shared counter
        pthread_mutex_unlock(&counter_mutex); // Unlock the mutex after updating 
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    pthread_mutex_init(&counter_mutex, NULL); 
    // Create multiple threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_counter, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", counter); // Print the final counter value
    return 0;
}

/*
* *** MY OUTPUT *** 
jared@xyz:~/Programming/OperatingSystems-COP4610/c-practice$ ./counter
Final counter value: 170795
jared@xyz:~/Programming/OperatingSystems-COP4610/c-practice$ ./counter
Final counter value: 148135
jared@xyz:~/Programming/OperatingSystems-COP4610/c-practice$ ./counter
Final counter value: 235149
jared@xyz:~/Programming/OperatingSystems-COP4610/c-practice$ n counter.c *** make mutex modifications ***
jared@xyz:~/Programming/OperatingSystems-COP4610/c-practice$ gcc -o counter counter.c
jared@xyz:~/Programming/OperatingSystems-COP4610/c-practice$ ./counter
Final counter value: 1000000
*/
