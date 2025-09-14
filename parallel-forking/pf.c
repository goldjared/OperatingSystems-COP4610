#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

// function prototype for compiler
void childTaskPrint(int childTask, int n, int totalChilds);

// get my name 

// fibonacci of n

// get the first n letters of alphabet

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
        return NULL; // memory allocation failed
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
	    "Calculating the sum of ", 
	    "Calculating the fibonacci number of ",
	    "Calculating the length of \"Jared Gold\"" 
	};

	pid_t pid = getpid();
	printf("[Child %d - PID: %d] Performing Task: %s%s.\n%s", childTask, (int) pid, strNameList[childTask - 1], num_str, (childTask == totalChilds ? "\n" : ""));
}

int main(void) {
	// general constant number for functions
	const int n = 10;

	int status = 0;
	int childCount;

	// error handle on > 5 or < 1 or 0
	printf("Input a number of child processes to create (1-5): ");
	scanf("%d", &childCount);

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
			if(i == 0) {

				childTaskPrint(1, n, childCount);
				int* counted = counter(n);
				size = n;
				write(fd[i][1], &size, sizeof(int));
				write(fd[i][1], counted, n * sizeof(int));
				free(counted);
			} else if(i == 1) {
				childTaskPrint(2, n, childCount);
				int* powerChildsToN = powerOfChildsNumToN(childCount, n);
				size = 1;
				write(fd[i][1], &size, sizeof(int));
				write(fd[i][1], powerChildsToN, sizeof(int));
				free(powerChildsToN);
			}

			exit(0);
		}
	}

	pid_t pid;

	for(int i = 0; i < childCount; i++) {
		close(fd[i][1]);         // parent closes write end for pipe i
		pid = waitpid(pids[i], &status, 0);  // wait for child i
		//
//		pid = wait(&status);
		int thisChild;
		read(fd[i][0], &thisChild, sizeof(thisChild));

		int incomingSize;
		read(fd[i][0], &incomingSize, sizeof(int));

		int* received = malloc(incomingSize * sizeof(int));
		//int received[incomingSize];
		read(fd[i][0], received, incomingSize * sizeof(int));
		//read(fd[0], received, incomingSize * sizeof(int));

		printf("[Parent - PID: %d] Child %d (PID: %d) completed it's task, result: ", parent, thisChild, pid);

		for (int i = 0; i < incomingSize; i++) {
		    printf("%d", received[i]);
		    if (i < incomingSize - 1) {
			printf(" ");
		    }
		}
		printf("\n");

		free(received);

		//childCount--;
	}

	return 1;
}
