#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char*argv[]){
	if (argc < 2){
		perror("No command");
		return 1;
	}

	pid_t procId = fork();
	if (procId == -1){
		perror("Something went wrong in fork");
		return 1;
	}
	if (procId == 0){
		execvp(argv[1], argv + 1);
	}
	int stat;
	wait(&stat);
	printf("Code: %d\n", stat);
	return 0;
}
