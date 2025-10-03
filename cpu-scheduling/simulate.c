#include <stdio.h>

#define TIME_QUANTUM 4;

struct simulatedProc {
	int id;
	int arrivalTime;
	int burstTime;
};

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
	}


	return 1;
}
