#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REF_STRING_SIZE 100

int refStringBuffer[MAX_REF_STRING_SIZE];
int refStringSize = 0;
int frames = 0;

void simulateLRU(int numFrames, const int* refString, int refSize);
void simulateFIFO(int numFrames, const int* refString, int refSize);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <reference_file.txt> <amount of frames>\n", argv[0]);
		return 1;
	}
	// validate frames
	if (sscanf(argv[2], "%d", &frames) != 1 || frames < 1 || frames > 10) {
		printf("Invalid frame count. Must be an integer between 1 and 10.\n");
		return 1;
	}

	const char *refFilename = argv[1];

	// build refString from file
	FILE *fp = fopen(refFilename, "r");
	if (fp == NULL) {
		printf("Error accessing file with name %s", refFilename);
		return -1;
	}


	// start count digits in refstringfile
	int i = 0;
	int page_num;
	while (i < MAX_REF_STRING_SIZE && fscanf(fp, "%d", &page_num) == 1) {
		refStringBuffer[i++] = page_num;
	}

	if (i == 0) {
		fprintf(stderr, "Error: Reference file invalid.\n");
	}

	fclose(fp);

	int refStringSize = i;
	if (refStringSize <= 0) {
 		fprintf(stderr, "Error: Reference file invalid.\n");
		return 1;
	}


	printf("\n======FIFO Page Replacement Simulation======\n");
	simulateFIFO(refStringBuffer, refStringSize);

	printf("\n======LRU Page Replacement Simulation======\n");
	simulateLRU(refStringBuffer, refStringSize);

	return 0;
}

void simulateFIFO(const int* refString, int refSize) {
	int frameArray[frames];
	int pageFaults = 0;
	int replacePointer = 0; 

	// Initialize all frames to a value that signifies empty, e.g., -1
	for (int i = 0; i < frames; i++) {
		frameArray[i] = -1;
	}

	for (int i = 0; i < refSize; i++) {
		int currentPage = refString[i];
		int hit = 0;

		// Search for a match in the frame array
		for (int j = 0; j < frames; j++) {
			if (frameArray[j] == currentPage) {
				hit = 1; // Match found, continue
				break;
			}
		}

		if (!hit) {

			// Page Fault since no match 
			pageFaults++;
			
			// Replace the page at the current FIFO pointer location
			frameArray[replacePointer] = currentPage;
			
			// set pointer to the next frame (circularly with mod)
			replacePointer = (replacePointer + 1) % frames;
		} 
	}

	printf("FIFO: %d page faults\n", pageFaults);
	printf("Final state of memory: ");
	// iter to ref size for the case of more frames than ref input
	for (int i = 0; i < frames; i++) {
		printf("%d%s", frameArray[i], (i == frames - 1 ? "" : " "));
	}
	printf("\n");
}

void simulateLRU(const int* refString, int refSize) {
	int frameArray[frames];
	int pageFaults = 0;

	// for lru, lowest num = LRU
	int lastUsed[frames]; 

	// init frames at -1 when empty
	for (int i = 0; i < frames; i++) {
		frameArray[i] = -1;
		lastUsed[i] = 0; 
	}


	for (int i = 0; i < refSize; i++) {
		// curr num we are on from refString
		int currentPage = refString[i];

		int hitIndex = -1;

		// search for a match within curr frames
		for (int j = 0; j < frames; j++) {
			if (frameArray[j] == currentPage) {
				hitIndex = j;
				break;
			}
		}

		// valid hit index exists
		if (hitIndex != -1) {
			// set to i, lower i = LRU (in the lastUsed arr)
			lastUsed[hitIndex] = i;
		} else {
			// Page Fault (No match)
			pageFaults++;
			int replaceIndex = -1;
			int minUsedTime = refSize + 1; // A value larger than any possible index 'i'
			
			// see if any frame is empty
			for (int j = 0; j < frames; j++) {
			    if (frameArray[j] == -1) {
				replaceIndex = j;
				break;
			    }
			}
			
			// no empty frame, at this point we find lru
			if (replaceIndex == -1) {
			    for (int j = 0; j < frames; j++) {
				if (lastUsed[j] < minUsedTime) {
					minUsedTime = lastUsed[j];
					replaceIndex = j;
				}
			    }
			}
			
			// replacement
			frameArray[replaceIndex] = currentPage;
			// update lastUsed with i, to get the larger i since lru will be lowest num
			lastUsed[replaceIndex] = i;
		}
	}

	printf("LRU: %d page faults\n", pageFaults);
	printf("Final state of memory: ");
	for (int i = 0; i < frames; i++) {
		printf("%d%s", frameArray[i], (i == frames - 1 ? "" : " "));
	}
	printf("\n");
}
