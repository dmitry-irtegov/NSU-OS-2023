#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char* argv[]){

	if(argc != 2){
		fprintf(stderr, "No or too many arguments.\n");
		return -1;
	}

	pid_t pid;

	pid = fork();

	if(pid == -1){
		perror("Fail fork");
		return -1;
	}
	if(pid == 0){
		execl("/usr/bin/cat", "cat", argv[1], NULL);
		perror("Exec fail");
		return -1;
	}
	if(pid > 0){
		if(wait(NULL) == -1){
			perror("Error wait");
			return -1;
		}
		printf("THIS TEXT ENTER PARENT!\n");
	}

	return 0;
}
