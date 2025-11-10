/*
*
* INSTRUCTIONS: expected filename is 'data.txt' -> 
* example file contents:
* 0 1 2 3
* 1 9 8 2
* 5 2 2 2
* 1 3 0 7
* 7 7 3 5
*
*
* DEBUG:
* * Had a bug where valid requests were denied and some 'need' values turned negative.  
* utilized this article to ref https://www.geeksforgeeks.org/operating-systems/bankers-algorithm-in-operating-system-2/  
* , with this ref I double checked the algo orders and found
* 1) Check if Request ≤ Need  
* 2) Check if Request ≤ Available  
* 3) TENTATIVELY allocate, then run safety check  
* 4) Roll back if unsafe  
* I had steps 1–3 mixed up, and was able to debug with prints and rearrange my ordering. 
* 
*
* another issue I ran into was my safety checker, I forgot to increment the counter after a customer was completed. this led to always being an unsafe state. The way I debugged was printing all the contents in that function. I noticed right away the counter was never changing.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5 // n
#define NUMBER_OF_RESOURCES 4 // m
int available[NUMBER_OF_RESOURCES]; //  Available[ j ] = k means there are 'k' instances of resource type Rj
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 
//  Max[ i, j ] = k means process Pi may request at most 'k' instances of resource type Rj.
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
//  Allocation[ i, j ] = k means process Pi is currently allocated 'k' instances of resource type Rj
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
// need = max - allocation


void display() {
	printf("============================\n");
	printf("System State\n");
	printf("Available:\n");
	printf("[");
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
		printf("%d", available[i]);
		if (i < NUMBER_OF_RESOURCES - 1)
			printf(", ");
	}
	printf("]\n\n");

	printf("Maximum:\n");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
		printf("P%d: [", i);
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
			printf("%d", maximum[i][j]);
			if (j < NUMBER_OF_RESOURCES - 1)
				printf(", ");
		}
		printf("]\n");
	}
	printf("\n");

	printf("Allocation:\n");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
	    printf("P%d: [", i);
	    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
		printf("%d", allocation[i][j]);
		if (j < NUMBER_OF_RESOURCES - 1)
		    printf(", ");
	    }
	    printf("]\n");
	}
	printf("\n");

	printf("Need:\n");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
	    printf("P%d: [", i);
	    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
		printf("%d", need[i][j]);
		if (j < NUMBER_OF_RESOURCES - 1)
		    printf(", ");
	    }
	    printf("]\n");
	}

	printf("============================\n\n");
}

// safe state check
// check for the customer id, each req resource. if any resource > need, auto reject
// Function to check if the system is in a safe state
int isSafe() {
	int work[NUMBER_OF_RESOURCES];
	int finish[NUMBER_OF_CUSTOMERS] = {0}; 	
	int count = 0;

	// set work to avail
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
		work[i] = available[i];
	}

	// find any customer proc that can complete
	while (count < NUMBER_OF_CUSTOMERS) {
		int found = 0;
		for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
			if (finish[i] == 0) { // if not yet finished
				int canFinish = 1;
				for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
					if (need[i][j] > work[j]) {
						canFinish = 0;
						break;
					}
				}
			if (canFinish) {
				// This process can finish - simulate releasing its resources
				for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
					work[j] += allocation[i][j];
				}
				finish[i] = 1;
				count++;
				found = 1;
			}
		}
	    }

	    if (!found) {
		// No process could finish - unsafe state
		printf("System is NOT in a safe state.\n");
		return 0;
	    }
	}

	printf("System is in a SAFE state.\n");
	
	return 1;
}


int requestResources(int customer_id, int request[]) {
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
        if (request[j] > need[customer_id][j]) {
            printf("ERROR: Request exceeding customer's need.\n");
            return 0;
        }

        if (request[j] > available[j]) {
            printf("ERROR: Resources NOT available.\n");
            return 0;
        }
    }

    // allocate resources
    for (int k = 0; k < NUMBER_OF_RESOURCES; k++) {
        available[k] -= request[k];
        allocation[customer_id][k] += request[k];
        need[customer_id][k] -= request[k];
    }

    // after we allocate above, check for safe state. 
    if (isSafe()) {
        printf("Request granted. System is in a safe state.\n");
        return 1;
    } else {
	// for the case of isSafe false, we need to rollback the above allocation
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            available[j] += request[j];
            allocation[customer_id][j] -= request[j];
            need[customer_id][j] += request[j];
        }
        printf("Request denied. System would be left in an unsafe state.\n");
        return 0;
    }
}

int release(int customerId, int release[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
	allocation[customerId][i] -= release[i];
	available[i] += release[i];
	need[customerId][i] += release[i];
    }

    printf("Resources released successfully.\n");
    return 1;
}

int main(int argc, char *argv[]) {
	printf("Welcome to the Banker's Algorithm Simulation.\n");
	// validate len
	if (argc != 5) {
		printf("Usage: %s <resource1> <resource2> <resource3> <resource4>\n", argv[0]);
		return 1;
	}

	printf("Initializing System with Resources:\n");
	printf("Available: [");
	for (int i = 1; i < argc - 1; i++) {
		printf("%s, ", argv[i]);
	}
	printf("%s]\n", argv[argc - 1]);

	// set available arr with passed resources
	for (int i = 1; i < argc; i++) {
		available[i - 1] = atoi(argv[i]);
	}

	// start process max req file
	printf("Reading maximum request file...\n");

	FILE *fp = fopen("data.txt", "r");
	if (fp == NULL) {
	    perror("Error accessing file");
	    return 1;
	}

	// write each customer (i) max request for each resource
	for (int i = 0; i < 5; i++) {
		// Read 4 integers per line
		if (fscanf(fp, "%d %d %d %d", 
			   &maximum[i][0], &maximum[i][1], &maximum[i][2], &maximum[i][3]) != 4) {
			fprintf(stderr, "Error reading line %d\n", i + 1);
			fclose(fp);
			return 1;
		}

		// init allocation and need
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
			// initially allocation is 0, and the need is same as the max (max - 0)
			allocation[i][j] = 0;
			need[i][j] = maximum[i][j] - allocation[i][j];
		}
	}


	fclose(fp);
	printf("Maximum requests initialized successfully.\n");
	// end process max req file
	
	// start command processing
	printf("Please enter commands:\n");
	printf("- 'RQ customer_id r1 r2 r3 r4' to request resources\n");
	printf("- 'RL customer_id r1 r2 r3 r4' to release resources\n");
	printf("- '*' to display the current system state\n");
	printf("- 'exit' to quit\n");


	char input[100];

	while (1) {
		printf("> "); // Prompt for input
		

		if (fgets(input, sizeof(input), stdin) == NULL) {
			// input error, re try
			continue;
		}

		// tokenize user input, first token = command
		char *command = strtok(input, " "); 
		if (command == NULL) {
			continue; 
		}
		// rmv trail new line
		input[strcspn(input, "\n")] = 0;


		if (strcmp(command, "exit") == 0) {
			printf("Exiting...\n");
			break;
		}
		else if (strcmp(command, "RQ") == 0) {
			// t1 is customer id
			char *token1 = strtok(NULL, " "); 
			char *token2 = strtok(NULL, " ");
			char *token3 = strtok(NULL, " ");
			char *token4 = strtok(NULL, " ");
			char *token5 = strtok(NULL, " ");

			if (token1 && token2 && token3 && token4) {
				int customer_id = atoi(token1);
				int req[NUMBER_OF_RESOURCES] = {atoi(token2), atoi(token3), atoi(token4), atoi(token5)};
				
				if (customer_id < 0 || customer_id >= NUMBER_OF_CUSTOMERS) {
				    printf("Invalid customer ID (VALID: 0-4.\n");
				    continue;
				}

				requestResources(customer_id, req);
			} else {
				printf("Invalid RQ format. Follow format: RQ <customer> <a> <b> <c> <d>\n");
			}
		}
		else if (strcmp(command, "RL") == 0) {
		    char *token1 = strtok(NULL, " ");
		    char *token2 = strtok(NULL, " ");
		    char *token3 = strtok(NULL, " ");
		    char *token4 = strtok(NULL, " ");
		    char *token5 = strtok(NULL, " ");

		    if (token1 && token2 && token3 && token4 && token5) {
			int customer_id = atoi(token1);
			int rel[NUMBER_OF_RESOURCES] = {atoi(token2), atoi(token3), atoi(token4), atoi(token5)};

			release(customer_id, rel);
		    } else {
			printf("Invalid RL format. Use: RL <customerId> <r1> <r2> <r3> <r4>\n");
		    }

		} else if (strcmp(command, "*") == 0) {
			display();
		}
		else {
			printf("Unknown command: %s\n", command);
		}
	}
	
	return 0;
}


