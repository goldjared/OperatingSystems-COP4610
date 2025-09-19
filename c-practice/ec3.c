#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* perform_work(void* arguments){
    printf("Thread Created.\n");
    return NULL;
}

int main(void) {
	
	pthread_t thread;
	int thread_arg = 0;
	pid_t pid;  

	// create process 1, parent and one child run after this
	pid = fork();  
	// p0 p1

	if (pid == 0) { /* child process */  

	// create process child 2 *2 child processes running after this fork, from within this block*
	// p1
	fork();  
	// p1 p2
	pid_t child_proc = getpid();
	printf("[A]child process pid: %d\n", child_proc);
	// each process (2) creates a thread, 2 threads
	pthread_create(&thread, NULL, perform_work, NULL); 

	} 

	// processes: p0, p1, p2; each fork here -> +3 more processes 
	fork();  

	// processes: p0, p1, p2, p3, p4, p5 
	return 1;

}

//    How many unique processes are created?parent 1, children: 5 -> 6 total processes 
//    How many unique threads are created?  2 threads


