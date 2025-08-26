#include <stdio.h>
/*
Write a C program that:

    Reads 10 integers from the user into an array.

    Sorts the array in ascending order using bubble sort.

    Prints the sorted array.
*/
const int size = 10;

void storeIntegersToArray(int array[size]) {
	printf("Enter %d number(s)\n", size);
	for(int i = 0; i < size; i++) {
		printf("Enter your number: ");
		scanf("%d", &array[i]);
	}
}

void bubbleSort(int array[size]) {
	// loop over array, then inner loop which swap i j on each iter, if no swaps occur, it is sorted
	int swapped = 0;
	for(int i = 0; i < size; i++) {
		swapped = 0;
		for(int j = 1; j < size; j++) {
			if(array[j] < array[j - 1]) {
				int copySmaller = array[j];
				array[j] = array[j - 1];
				array[j - 1] = copySmaller;
				swapped = 1;
			}
		}
		if(swapped == 0) break;
	}
}

void printArray(int array[size]) {
	for(int i = 0; i < size - 1; i++) {
		printf("%d, ", array[i]);
	}
	printf("%d\n", array[size - 1]);
}

int main() {
	int myArray[size];
	storeIntegersToArray(myArray);
	printf("Array before sorting:\n");
	printArray(myArray);
	bubbleSort(myArray);
	printf("Array after sorting:\n");
	printArray(myArray);
	return 0;
}

