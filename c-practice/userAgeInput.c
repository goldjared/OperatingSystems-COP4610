#include <stdio.h>
/*
 * Create a program that asks the user for their age as input. If the age is greater than or equal to 18, 
 * the program should output "You are an adult." Otherwise, it should output "You are a minor." 
*/
void getUserAge() {
	int age;
	printf("Enter your age: ");
	scanf("%d", &age);
	printf(age>=18 ? "You are an adult.\n" : "You are a minor.\n");
}

int main() {
	getUserAge();
	return 0;
}


