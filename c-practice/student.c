/*
    Write a C program that defines a struct named "Student" with the following members:
        "name" of type char array (maximum 50 characters)
        "age" of type int
        "grade" of type char

    In the main function, create an instance of the "Student" struct and initialize its members with appropriate values.

    Print out the details of the student using printf in the following format:

Name: [name]

Age: [age]

Grade: [grade]
*/
#include <stdio.h>
#include <string.h>

struct Student {
	char name[30];
	int age;
	char grade;
};

int main() {
	struct Student student;
	/*
		* note: could also do student = {"name", age, 'grade'}
	*/
	strcpy(student.name, "Jared Gold");
	student.age = 10000;
	student.grade = 'A';
	printf("Name: %s\n\n", student.name);
	printf("Age: %d\n\n", student.age);
	printf("Grade: %c\n", student.grade);
	return 0;
}
