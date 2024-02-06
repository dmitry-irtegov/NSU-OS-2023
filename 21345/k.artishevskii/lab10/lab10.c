#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	if(argc < 2){
		fprintf(stderr, "Invalid number arguments\n");
		return -1;
	}
	pid_t pid = fork();
	if(pid == -1){
		perror("Fail fork");
		return -1;
	}
	if(pid == 0){
		execvp(argv[1], argv + 1);
		perror("Fail exec");
		return -1;
	}
	if(pid > 0){
		int status;
		pid_t ret = wait(&status);
		if(ret == -1){
			perror("Error after wait");
			return -1;
		}
		if(WIFEXITED(status) != 0){
			printf("Exit code = %d\n", WEXITSTATUS(status));
			return 0;
		}
		if(WIFSIGNALED(status) != 0){
			printf("Signal code = %d\n", WTERMSIG(status));
			return 0;
		}
	}
	return 0;
}
