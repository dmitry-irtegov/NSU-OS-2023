<<<<<<< HEAD
#include <stdio.h> // perror
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t process_id;
    printf("Process ID: %d\n", getpid());

    process_id = fork();

    switch (process_id) {
        case -1:
            perror("Fork Error\n");
            return -1;
        case 0:
	    printf("Child start\n");
            printf("Child Process ID: %d\n", getpid());
	    if(!argv[1]){
	 	perror("No arg\n");
		exit(0);
		}
            execlp("cat", "cat", argv[1], NULL);

        default: {
            int child_stat;
            printf("Parent start\n");
            wait(&child_stat);
            printf("Parent end\n");
        }

    }

    return 0;
}
