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

	// Print Maximum
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

	// Print Allocation
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

	// Print Need
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


	char input[100]; // Buffer to store user input

	while (1) {
		printf("> "); // Prompt for input
		

		if (fgets(input, sizeof(input), stdin) == NULL) {
			// Handle potential input error or EOF
			continue;
		}

		// Split into tokens
		char *cmd = strtok(input, " "); // first token = command

		if (cmd == NULL)
			continue; // skip empty line

		

		// Remove trailing newline character if present
		input[strcspn(input, "\n")] = 0;


		// Handle commands
		if (strcmp(cmd, "exit") == 0) {
			printf("Exiting...\n");
			break;
		}
		else if (strcmp(cmd, "RQ") == 0) {
			// Extract the next 4 integers
			char *t1 = strtok(NULL, " "); // this is the customer ID
			char *t2 = strtok(NULL, " ");
			char *t3 = strtok(NULL, " ");
			char *t4 = strtok(NULL, " ");
			char *t5 = strtok(NULL, " ");

			if (t1 && t2 && t3 && t4) {
				int a = atoi(t1);
				int b = atoi(t2);
				int c = atoi(t3);
				int d = atoi(t4);
				int e = atoi(t5);
				//RQ(a, b, c, d);
				printf("RQ command, vals: %d %d %d %d %d\n", a, b, c, d, e);
			} else {
				printf("Invalid RQ format. Follow this format: RQ <a> <b> <c> <d>\n");
			}
		}
		else if (strcmp(cmd, "RL") == 0) {
			printf("RL command - releasing resources (example).\n");
		} else if (strcmp(cmd, "*") == 0) {
			display();
		}
		else {
			printf("Unknown command: %s\n", cmd);
		}
	}
	
	return 0;
}


