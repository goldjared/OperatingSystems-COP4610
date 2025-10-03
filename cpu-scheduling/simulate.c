#include <stdio.h>
#include <stdlib.h>

#define TIME_QUANTUM 4;

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
/*
 *
	** FCFS:
* 	list input is exact order to print/execute on
* 	create 2d array of [n][2], where each arr will store wait, turnaround time
*	
*	int time = 0
*	double waitSum = 0;
*	double turnaroundSum = 0;
	init time to first arrival time
* 	iterate to n (n is len proc list)
* 		process_date[i][0] = time - arrival (set wait time)
*
* 		time += burst_time
* 		process_date[i][1] = process_date[i][0] + burst_time 
* 		waitSum += process_date[i][0]
* 		turnAroundSum += process_date[i][1]
*
* 	print process list
* 	print waitSum / n
* 	print turnaroundSum / n
*
*
	*/

	// 2d array wait time, turnaround time, where i+1 = process id
	int processData[size][2]; 
	int time = processList[0].arrivalTime;
	double waitSum = 0;
	double turnAroundSum = 0;

	int execLog[100];
	int execCount = 0;
	for(int i = 0; i < size; i++) {
		// bug here for arrival 0, 1, 30
		processData[i][0] = time + processList[i].arrivalTime;
		time += processList[i].burstTime;
		processData[i][1] = processData[i][0] + processList[i].burstTime;
		waitSum += processData[i][0];
		turnAroundSum += processData[i][1];

		execLog[execCount++] = processList[i].id;
	}
	printf("Simulating FCFS...\n");
	printOrder(execLog, execCount);
	printf("Avg Waiting Time: %.2f\n", waitSum / size);
	printf("Avg Turnaround Time: %.2f\n", turnAroundSum / size);

}

int main(void) {

	// get total processes to simulate
	int processCount = 0;
	printf("Enter a number of processes (1-5): ");
	scanf("%d", &processCount);

	// if invalid value entry exit program
	if(processCount < 1 || processCount > 5) {
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
			procList[i].arrivalTime > 10000 ||
			procList[i].burstTime < 1 ||
			procList[i].burstTime > 10000
		) {
		    printf("Invalid input. Please enter values between 0-10000 for Arrival, and 1-10000 for Burst.\n");
		    i--; // retry current process
		}
		
		printf("\n");
	} // end for

	// sort proc list based on arrival times
	qsort(procList, processCount, sizeof(struct simulatedProc), compareArrival);
	// init FCFS
	fcfs(procList, processCount);


	return 1;
}
