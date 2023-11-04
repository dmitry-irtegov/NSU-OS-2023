#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, int *argv[]){
	if (argc < 2){
		perror("No file");
		return 1;
	}
	char* arg[argc+1];
	memcpy(arg, argv, argc*sizeof(char*));
	arg[0] = "cat";
	arg[argc] = NULL;

	pid_t procId = fork();
	if (procId == -1){
		perror("fork failured");
		return 1;
	}
	if (procId == 0) {
		execv("/bin/cat", arg);
		perror("Something went wrong in execv");
		return 1;
	}
	printf("before\n");
	wait(0);
	printf("after\n");
	return 0;
}
