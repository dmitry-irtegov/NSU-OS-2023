#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char* argv[]){
	if (argc < 2){
		perror("Not enough arguments, at least 1 required. \n");
		return -1;
	}
	pid_t fork_pid;
	fork_pid = fork();
	if (fork_pid == -1){
		perror("Failed to fork. \n");
		return -1;
	}

	if (fork_pid == 0){
		printf("Child working. \n");
		if (execvp(argv[1], argv + 1) == -1){
			perror("Execution failed. \n");
			return -1;
		}
	}

	int exit_status;
	if (wait(&exit_status) == -1){
		perror("Wait failed. \n");
		return -1;
	}

	if (WIFEXITED(exit_status)==0){
		perror("Child teminated unexpectedly.");
		return -1;
	}
	printf("Exeqution of process (pid): %d ended with status: %d \n", fork_pid, WEXITSTATUS(exit_status));
	return 0;
}
