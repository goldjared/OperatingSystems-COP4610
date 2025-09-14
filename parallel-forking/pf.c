#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int childCount;
	printf("Input a number of child processes to create (1-5):\n");
	scanf("%d", &childCount);
	// error handle on > 5 or < 1 or 0

	pid_t parent = getpid();
	printf("Parent process (PID: %d) is creating %d child processes.\n", (int) parent, childCount);

	return 1;
}
