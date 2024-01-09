#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    
    pid_t process_id = fork();
    
    if (process_id < 0){
        perror("Fork failure");
        exit(1);
    }
    
    else if (process_id == 0) {
        execlp("/bin/cat", "cat", "file.txt", NULL);
    }
    
    else {
        printf("Parent process printing some text\n");
        wait(NULL);
        printf("Parent process printing text after child process execution\n");
    }
    return 0;
}

