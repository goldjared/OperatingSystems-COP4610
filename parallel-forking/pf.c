#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

// function prototype for compiler
void childTaskPrint(int childTask, int n, int totalChilds);

int* countInReverse(int n) {
	int* arr = malloc(n * sizeof(int));
	if (arr == NULL) {
		return NULL;
	}

	for (int i = 10; i > 0; i--) {
		arr[n - i] = i;
	}

	return arr;
}

// fibonacci of n
int fib(int n) {
	// fib 3 = fib(2) + fib(1) 0 1 1
	if(n < 2) return n;
	return fib(n-2) + fib(n-1);
}

// create prefixed product array from 1 to n
int* prefixedProdArray(int n) {
	int* arr = malloc(n * sizeof(int));
	if (arr == NULL) {
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		arr[i] = i + 1;
	}

	// calc sums
	for (int i = 1; i < n; i++) {
		arr[i] *= arr[i - 1];
	}

	return arr;
}

// get the power of childsNum to n
int* powerOfChildsNumToN(int childNum, int n) {
	int* resultArr = malloc(sizeof(int));  // allocate space for 1 int
	*resultArr = 1;

	for (int i = 0; i < n; i++) {
		*resultArr *= childNum;
	}

	return resultArr;
}

// function returns a dynamically allocated array of integers
int* counter(int n) {
    // allocate memory for n integers
    int* arr = malloc(n * sizeof(int));
    if (arr == NULL) {
	// memory allocation failed
        return NULL; 
    }

    // fill the array with numbers from 1 to n
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    return arr;
}

void childTaskPrint(int childTask, int n, int totalChilds) {
	// buffer for integer's string representation
	char num_str[10]; 

	if(n == -1) 
		num_str[0] = '\0';
	else 
		sprintf(num_str, "%d", n); 

	const char *strNameList[5] = { 
	    "Counting from 1 to ",
	    "Calculating raising childs^",
	    "Calculating the prefixed product arr of 1 ... ", 
	    "Calculating the fibonacci number of ",
	    "Counting down from " 
	};

	pid_t pid = getpid();
	printf("[Child %d - PID: %d] Performing Task: %s%s.\n%s", childTask, (int) pid, strNameList[childTask - 1], num_str, (childTask == totalChilds ? "\n" : ""));
}

int main(void) {
	clock_t start_time = clock();
	// general constant number for functions
	const int n = 10;

	int status = 0;
	int childCount;

	// error handle on > 5 or < 1 or 0
	printf("Input a number of child processes to create (1-5): ");
	scanf("%d", &childCount);

	// if invalid value entry exit program
	if(childCount < 1 || childCount > 5) {
		printf("Invalid Input, exiting.\n");
		return 1;
	}

	// pipes
	int fd[childCount][2];
	for (int i = 0; i < childCount; i++) {
	    if (pipe(fd[i]) == -1) {
		perror("pipe failed");
		exit(1);
	    }
	}

	// create array of childCount to store child pids
	int pids[childCount];
	
	pid_t parent = getpid();
	printf("Parent process (PID: %d) is creating %d child processes.\n\n", (int) parent, childCount);

	// loop to childCount, create forks
	for(int i = 0; i < childCount; i++) {
		if((pids[i] = fork()) < 0) {
			// error
			perror("error\n");
			abort();
		} else if(pids[i] == 0) {
			// child process
			// close read end
			close(fd[i][0]);
			
			int size;
			// write child number
			int thisChild = i + 1;
			write(fd[i][1], &thisChild, sizeof(thisChild));
			childTaskPrint(i + 1, n, childCount);
			if(i == 0) {
				clock_t start_time = clock();
				int* counted = counter(n);
				clock_t end_time = clock();
				double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				printf("task1 counter() took %f seconds to execute.\n", time_taken);

				size = n;
				write(fd[i][1], &size, sizeof(int));
				write(fd[i][1], counted, n * sizeof(int));
				free(counted);
			} else if(i == 1) {
				clock_t start_time = clock();
				int* powerChildsToN = powerOfChildsNumToN(childCount, n);
				clock_t end_time = clock();
				double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				printf("task2 powerOfChildsNumToN() took %f seconds to execute.\n", time_taken);
				size = 1;
				write(fd[i][1], &size, sizeof(int));
				write(fd[i][1], powerChildsToN, sizeof(int));
				free(powerChildsToN);
			} else if(i == 2) {
				clock_t start_time = clock();
				int* prefixProdArr = prefixedProdArray(n);
				clock_t end_time = clock();
				double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				printf("task3 prefixProdArr() took %f seconds to execute.\n", time_taken);
				size = n;
				write(fd[i][1], &size, sizeof(int));
				write(fd[i][1], prefixProdArr, n * sizeof(int));
				free(prefixProdArr);
			} else if(i == 3) {
				clock_t start_time = clock();
				int f = fib(n);
				clock_t end_time = clock();
				double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				printf("task4 fibonacci() took %f seconds to execute.\n", time_taken);
				int fibStore[1];
				fibStore[0] = f;
				size = 1;
				write(fd[i][1], &size, sizeof(int));
				write(fd[i][1], &fibStore, sizeof(int));
			} else if(i == 4) {
				clock_t start_time = clock();
				int* reverseCount = countInReverse(n);
				clock_t end_time = clock();
				double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				printf("task5 countInReversecountInReverse() took %f seconds to execute.\n", time_taken);
				size = n;
				write(fd[i][1], &size, sizeof(int));
				write(fd[i][1], reverseCount, n * sizeof(int));
				free(reverseCount);
			}

			exit(0);
		}
	}

	pid_t wpid;

	// wait for all child proc complete
	while ((wpid = wait(&status)) > 0);

	for(int i = 0; i < childCount; i++) {
		// parent closes write end for pipe i
		close(fd[i][1]);         

		// ensure we are in parent
		pid_t parent = getpid();
		// child pid we are processing
		pid_t pid = pids[i];

		int thisChild;
		read(fd[i][0], &thisChild, sizeof(thisChild));

		int incomingSize;
		read(fd[i][0], &incomingSize, sizeof(int));

		int* received = malloc(incomingSize * sizeof(int));
		read(fd[i][0], received, incomingSize * sizeof(int));

		printf("[Parent - PID: %d] Child %d (PID: %d) completed it's task, result: ", parent, thisChild, pid);

		for (int i = 0; i < incomingSize; i++) {
		    printf("%d", received[i]);
		    if (i < incomingSize - 1) {
			printf(" ");
		    }
		}
		printf("\n");

		free(received);
	}
	clock_t end_time = clock();
        double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	printf("**program** took %f seconds to execute.\n", time_taken);

	return 1;
}
