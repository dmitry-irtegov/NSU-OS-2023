#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int main() {
    int fildes[2];
    if (pipe(fildes) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("fork");
            close(fildes[0]);
            close(fildes[1]);
            exit(EXIT_FAILURE);
        case 0: 
            close(fildes[1]);
            int in = fildes[0];
            char ch;
            while (read(in, &ch, 1)) {
                printf("%c", tolower(ch));
            }
            printf("\n");
            close(in);
            break;
        default:                  
            close(fildes[0]); 
            int out = fildes[1];
            char* message = "I'M TIRED";
            for (char* msgLen = message; *msgLen != '\0'; msgLen++)
            {
                if(write(out, msgLen, 1) == 0) {
                    perror("write");
                    close(out);
                    exit(EXIT_FAILURE);
                }
            }
            close(out);

            if (waitpid(pid, NULL, 0) == -1)
            {
                perror("wait");
                exit(EXIT_FAILURE);
            }
    }
    exit(EXIT_SUCCESS);
}
