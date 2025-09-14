#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// get my name 

// fibonacci of n

// get the first n letters of alphabet

// get the power of n

// function returns a dynamically allocated array of integers
int* counter(int n) {
    // allocate memory for n integers
    int* arr = malloc(n * sizeof(int));
    if (arr == NULL) {
        return NULL; // memory allocation failed
    }

    // fill the array with numbers from 1 to n
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    return arr;
}

int main(void) {
	// general constant number for functions
	const int n = 10;

	int status = 0;
	int childCount;

	// childs write their #
	int thisChild;

	// pipe
	int fd[2];
	pipe(fd);

	// create array of childCount to store child pids
	int pids[childCount];
	
	printf("Input a number of child processes to create (1-5):\n");
	scanf("%d", &childCount);
	// error handle on > 5 or < 1 or 0

	pid_t parent = getpid();
	printf("Parent process (PID: %d) is creating %d child processes.\n", (int) parent, childCount);

	// loop to childCount, create forks
	for(int i = 0; i < childCount; i++) {
		if((pids[i] = fork()) < 0) {
			// error
			perror("error\n");
			abort();
		} else if(pids[i] == 0) {
			// child process
			// close read end
			close(fd[0]);
			
			// write child number
			thisChild = i + 1;
			write(fd[1], &thisChild, sizeof(thisChild));
			if(i == 0) {
				int* counted = counter(n);
				write(fd[1], counted, n * sizeof(int));
				free(counted);
			}

			exit(0);
		}
	}

	// wait for all child procs to finish
	// we can assume we are parent here right? if we are parent our pids array should have all child pid ids
	pid_t pid;
	while(childCount > 0) {
		pid = wait(&status);

		// close write side
		close(fd[1]);

		int received[n];

		read(fd[0], &thisChild, sizeof(thisChild));
		read(fd[0], received, sizeof(received));

		printf("[Parent - PID: %d] Child %d (PID: %d) completed it's task, result: ", parent, thisChild, pid);

		for (int i = 0; i < n; i++) {
		    printf("%d", received[i]);
		    if (i < n - 1) {
			printf(" ");
		    }
		}
		printf("\n");

		childCount--;
	}

	return 1;
}
