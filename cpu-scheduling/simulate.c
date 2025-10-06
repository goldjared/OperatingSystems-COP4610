/*
* RELEVANT LINKS
* 
* - https://cplusplus.com/reference/cstdlib/qsort/
* Referenced to build the custom sort function on the process structure list
*
*/
#include <stdio.h>
#include <stdlib.h>

const int TIME_QUANTUM = 2;

struct simulatedProc {
	int id;
	int arrivalTime;
	int burstTime;
};

int compareArrival(const void * a, const void * b) {
	const struct simulatedProc *pa = (const struct simulatedProc *)a;
	const struct simulatedProc *pb = (const struct simulatedProc *)b;

	if (pa->arrivalTime < pb->arrivalTime) return -1;
	if (pa->arrivalTime > pb->arrivalTime) return 1;
	return 0;
}

void printOrder(int execLog[], int size) {
	printf("Order: ");
	for(int i = 0; i < size; i++) {
		printf("P%d", execLog[i]);
		if(i + 1 != size) printf(" -> ");
	}

	printf("\n");
}

void fcfs(struct simulatedProc processList[], int size) {

	printf("Simulating FCFS...\n");
	// 2d array wait time, turnaround time, where i+1 = process id
	int processData[size][2]; 
	// init to zero
	for(int i = 0; i < size; i++) {
		processData[i][0] = 0;
		processData[i][1] = 0;
	}
	int time = processList[0].arrivalTime;
	double waitSum = 0;
	double turnAroundSum = 0;

	int execLog[100];
	int execCount = 0;
	for(int i = 0; i < size; i++) {

		if (time < processList[i].arrivalTime) {
			// wait for process to arrive
			time = processList[i].arrivalTime; 
		}
		processData[i][0] = time - processList[i].arrivalTime;
		time += processList[i].burstTime;

		processData[i][1] = time - processList[i].arrivalTime;
		waitSum += processData[i][0];
		turnAroundSum += processData[i][1];

		execLog[execCount++] = processList[i].id;
	}

	printOrder(execLog, execCount);

	double avgWait = waitSum / size;
	printf("Avg Waiting Time: %.2f\n", avgWait);

	double avgTurnaround = turnAroundSum / size;
	printf("Avg Turnaround Time: %.2f\n", avgTurnaround);
}

void rr(struct simulatedProc processList[], int size) {

	printf("Simulating RR... (time quantum: %d)\n", TIME_QUANTUM);
	// index 0 -> waitTime 1 -> turnAroundTime
	int processData[size][2]; 
	int lastExecSpot[size];
	// build last execSpot
	for(int i = 0; i < size; i++) {
		processData[i][0] = 0;
		processData[i][1] = 0;
		lastExecSpot[i] = processList[i].arrivalTime;
	}
	int time = processList[0].arrivalTime;
	int execLog[500];
	int execCount = 0;
	int procCount = size;
	int curr=0;

	int iteratedSizeTimes = 0;
	while(procCount > 0) {
		if(iteratedSizeTimes < size) iteratedSizeTimes++;

		if(curr == size) curr=0;
		if(processList[curr].burstTime == 0 || processList[curr].arrivalTime > time) {
			if(iteratedSizeTimes == size && processList[curr].arrivalTime > time) {
				time = processList[curr].arrivalTime;
			}
			curr++;
			continue;
		}

		// log to execLog
		execLog[execCount++] = processList[curr].id;

		// set wait time, time - last exec
		if (time > lastExecSpot[curr]) {
			processData[curr][0] += time - lastExecSpot[curr];
		}

		// curr proc completes at this point
		if(processList[curr].burstTime <= TIME_QUANTUM) {
			// add burst to time
			time += processList[curr].burstTime;

			// turnAround = completion time - arrival 
			processData[curr][1] = time - processList[curr].arrivalTime;

			// set burst to 0
			processList[curr].burstTime = 0;
			procCount--;

		} else {
			processList[curr].burstTime -= TIME_QUANTUM;
			time += TIME_QUANTUM;
		}
		
		lastExecSpot[curr] = time;

		curr++;
	}

	double waitSum = 0;
	double turnAroundSum = 0;
	// build waitSum and turnAroundSum
	for(int i = 0; i < size; i++) {
		waitSum += processData[i][0];
		turnAroundSum += processData[i][1];
	}


	printOrder(execLog, execCount);

	double avgWait = waitSum / size;
	printf("Avg Waiting Time: %.2f\n", avgWait);

	double avgTurnaround = turnAroundSum / size;
	printf("Avg Turnaround Time: %.2f\n", avgTurnaround);
}

int main(void) {

	// get total processes to simulate
	int processCount = 0;
	printf("Enter a number of processes (1-10): ");
	scanf("%d", &processCount);

	// if invalid value entry exit program
	if(processCount < 1 || processCount > 10) {
		printf("Invalid Input, exiting.\n");
		return 1;
	}

	// build list of processes
	printf("Enter process details:\n");
	struct simulatedProc procList[processCount];
	for(int i = 0; i < processCount; i++) {
		procList[i].id = i + 1;

		// used for detect invalid arrival/burst input
		int arrivalRead = 0, burstRead = 0;

		printf("P%d: Arrival=", procList[i].id);
		arrivalRead = scanf("%d", &procList[i].arrivalTime);
		while (getchar() != '\n');  

		// if arrivalInput was invalid, skip this one and jump to error handle
		if(arrivalRead == 1) { 
			printf("P%d: Burst=", procList[i].id);
			burstRead = scanf("%d", &procList[i].burstTime);
			while (getchar() != '\n');
		}

		// if invalid value entry retry for this (i) 
		if (
			arrivalRead != 1 || 
			burstRead != 1 || 
			procList[i].arrivalTime < 0 ||
			procList[i].arrivalTime > 100 ||
			procList[i].burstTime < 1 ||
			procList[i].burstTime > 100
		) {
		    printf("Invalid input. Please enter values between 0-100 for Arrival, and 1-100 for Burst.\n");
		    i--; // retry current process
		}
		
		printf("\n");
	} // end for

	// sort proc list based on arrival times
	qsort(procList, processCount, sizeof(struct simulatedProc), compareArrival);
	// init FCFS
	fcfs(procList, processCount);
	printf("\n");
	// init RR
	rr(procList, processCount);

	return 0;
}
