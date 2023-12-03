#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t process_id;
    printf("Process ID: %d\n", getpid());

    process_id = fork();

    switch (process_id) {
        case -1:
            perror("Fork Error");
            return -1;
        case 0:
	    printf("Child start\n");
            printf("Child Process ID: %d\n", getpid());
	    if(!argv[1]){
	 	fprintf(stderr, "No arg\n");
		return -1;
	    }
            int exec_error = execlp("cat", "cat", argv[1], NULL);
	    if(exec_error == -1){
		perror("execlp Error");
            }
        default: {
            int child_stat;
            printf("Parent start\n");
            wait(&child_stat);
            printf("Parent end\n");
        }

    }

    return 0;
}
