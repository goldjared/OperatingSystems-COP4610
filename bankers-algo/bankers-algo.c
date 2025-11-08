#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_CUSTOMERS 5 // n
#define NUMBER_OF_RESOURCES 4 // m
int available[NUMBER_OF_RESOURCES]; //  Available[ j ] = k means there are 'k' instances of resource type Rj
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 
//  Max[ i, j ] = k means process Pi may request at most 'k' instances of resource type Rj.
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
//  Allocation[ i, j ] = k means process Pi is currently allocated 'k' instances of resource type Rj
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
// need = max - allocation


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

	int arr[5][4]; // 5 lines x 4 integers

	// write each customer (i) max request for each resource
	for (int i = 0; i < 5; i++) {
	    // Read 4 integers per line
	    if (fscanf(fp, "%d %d %d %d", 
		       &maximum[i][0], &maximum[i][1], &maximum[i][2], &maximum[i][3]) != 4) {
		fprintf(stderr, "Error reading line %d\n", i + 1);
		fclose(fp);
		return 1;
	    }
	}

	fclose(fp);
	printf("Maximum requests initialized successfully.\n");
	// end process max req file
	//
	// start command processing
	printf("Please enter commands:\n");
	printf("- 'RQ customer_id r1 r2 r3 r4' to request resources\n");
	printf("- 'RL customer_id r1 r2 r3 r4' to release resources\n");
	printf("- '*' to display the current system state\n");
	printf("- 'exit' to quit\n");
	return 0;
}


