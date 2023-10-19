#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	if(argc!=2){
		perror("not enough args");
		return 1;
	}
	pid_t pid;
	int status;

	if((pid=fork())<0){
		perror("couldn`t fork");
		return 1;
	}

	if (pid==0){
		if (execl("/bin/cat","cat",argv[1],NULL)==-1){
			perror("execl error");
			return 1;
		}
		
	}

	if ((pid  = wait(&status)) == -1){
		perror("wait error");
		return 1;
	}

	printf("some text / parrent process\n");
	
	return 0;
}
